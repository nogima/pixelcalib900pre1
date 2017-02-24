#include <iostream>
#include <map>
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CondFormats/CTPPSObjects/interface/CTPPSPixelGainCalibrations.h"
#include "CondFormats/DataRecord/interface/CTPPSPixelGainCalibrationsRcd.h"

//#include "RecoCTPPS/CTPPSPixelLocal/interface/RPixDetClusterizer.h"
#include "Geometry/VeryForwardGeometry/interface/CTPPSPixelTopology.h"

#include "RecoCTPPS/CTPPSPixelLocal/interface/CTPPSPixelGainCalibrationDBService.h"
#include "RecoCTPPS/CTPPSPixelLocal/interface/CTPPSPixelClusterProducer.h"

RPixDetClusterizer::RPixDetClusterizer(edm::ParameterSet const& conf):
  params_(conf), SeedVector_(0)
{
verbosity_ = conf.getParameter<int>("RPixVerbosity");
SeedADCThreshold_ = conf.getParameter<int>("SeedADCThreshold");
ADCThreshold_ = conf.getParameter<int>("ADCThreshold");
ElectronADCGain_ = conf.getParameter<double>("ElectronADCGain");
}

RPixDetClusterizer::~RPixDetClusterizer(){}

void RPixDetClusterizer::buildClusters(unsigned int detId, const std::vector<CTPPSPixelDigi> &digi, std::vector<CTPPSPixelCluster> &clusters)
{

 if(verbosity_) std::cout<<" RPixDetClusterizer "<<detId<<" received digi.size()="<<digi.size()<<std::endl;
if(verbosity_)  for(unsigned int i=0; i<digi.size();i++)std::cout<< digi[i].adc() << std::endl;


// creating a set of CTPPSPixelDigi's and fill it
// NOTE: this should be also the place where SiPixels do the Calibration, i.e. when they create the buffer via make_buffer method!!
 rpix_digi_set_.clear();
 rpix_digi_set_.insert(digi.begin(),digi.end());

// try to modify/calibrate digis here
 calib_rpix_digi_set_.clear();
 for( std::set<CTPPSPixelDigi>::iterator RPdit = rpix_digi_set_.begin(); RPdit != rpix_digi_set_.end();++RPdit){
   int row = (*RPdit).row();
   int column = (*RPdit).column();
   int adc = (*RPdit).adc();
   int electrons = calibrate(detId,adc,row,column);
//calibrate digi and store the new ones (it still does nothing!!)
   RPixCalibDigi calibDigi(row,column,adc,electrons);
   calib_rpix_digi_set_.insert(calibDigi);


 }
 if(verbosity_) std::cout<<" RPix set size = "<<calib_rpix_digi_set_.size()<<std::endl;
// storing the seeds
 SeedVector_.clear();
 std::set<RPixCalibDigi>::iterator RPDit;
 for(RPDit = calib_rpix_digi_set_.begin(); RPDit!=calib_rpix_digi_set_.end();++RPDit){
       if(verbosity_) std::cout<<"adc    " << (*RPDit).adc()<< std::endl;
       if(verbosity_) std::cout<<"ele    " << (*RPDit).electrons()<< std::endl;

   if((*RPDit).electrons() > SeedADCThreshold_*ElectronADCGain_){
     
   // storing the seed if above threshold
     SeedVector_.push_back(*RPDit);

   } 
 }
 if(verbosity_) std::cout<<" SeedVector size = "<<SeedVector_.size()<<std::endl;



//----
// Looping on the seeds to make a cluster around the seed

 for(std::vector<RPixCalibDigi>::iterator SeedIt = SeedVector_.begin(); SeedIt!=SeedVector_.end();++SeedIt){
   
// testing find method ... to be removed
//   std::set<RPixCalibDigi>::iterator RPDit3 = calib_rpix_digi_set_.find( *SeedIt );
//   if(verbosity_) std::cout<<"    " <<  (*RPDit3).adc()<< std::endl;

// make cluster around the seed

   make_cluster(*SeedIt, clusters);

// to be finished ........


 }

}

void RPixDetClusterizer::make_cluster(RPixCalibDigi aSeed,  std::vector<CTPPSPixelCluster> &clusters ){

// THIS DEPENDS ON SENSOR TOPOLOGY!!!

// CHECK ALSO FOR CALIBRATION

  int max_rows_in_sensor=160;
  int max_cols_in_sensor=156;


// creating a temporary cluster

  tempCluster atempCluster;

// filling the cluster with the seed
 atempCluster.addPixel(aSeed.row(),aSeed.column(),aSeed.electrons());
 calib_rpix_digi_set_.erase( aSeed );


 while ( ! atempCluster.empty()) {
   //This is the standard algorithm to find and add a pixel
     auto curInd = atempCluster.top(); atempCluster.pop();
     for ( auto c = std::max(0,int(atempCluster.col[curInd])-1); c < std::min(int(atempCluster.col[curInd])+2,max_cols_in_sensor) ; ++c) {
	 for ( auto r = std::max(0,int(atempCluster.row[curInd])-1); r < std::min(int(atempCluster.row[curInd])+2,max_rows_in_sensor); ++r)  {

//
	   for(std::set<RPixCalibDigi>::iterator RPDit4 = calib_rpix_digi_set_.begin(); RPDit4 != calib_rpix_digi_set_.end(); ){
	     if( (*RPDit4).column() == c && (*RPDit4).row() == r && (*RPDit4).electrons() > ADCThreshold_*ElectronADCGain_ ){
	     
	       if(!atempCluster.addPixel( r,c,(*RPDit4).electrons() )) {goto endClus;}
	       RPDit4 =  calib_rpix_digi_set_.erase(RPDit4);

	     }else{
	       ++RPDit4;
	     }

	   }

	 }
       }
	     
 }  // while accretion


  endClus:
     //  SiPixelCluster cluster(atempCluster.isize,atempCluster.adc, atempCluster.x,atempCluster.y, atempCluster.xmin,atempCluster.ymin);


  if(verbosity_) atempCluster.printCluster();
  CTPPSPixelCluster cluster(atempCluster.isize,atempCluster.adc, atempCluster.row,atempCluster.col, atempCluster.rowmin,atempCluster.colmin);
  clusters.push_back(cluster);

// SPLITTING LARGE CLUSTERS TO BE DONE

}


int RPixDetClusterizer::calibrate(unsigned int detId, int adc, int row, int col){

  CTPPSPixelGainCalibration DetCalib = thecalibration.pPixelGainCalibrations->getGainCalibration(detId);
  bool isnoisy_g=false;  
  bool isdead_g=false;
  bool isnoisy_p=false;
  bool isdead_p=false;
// double -> float
  float gain = DetCalib.getGain(col,row,isdead_g,isnoisy_g);
  float pedestal = DetCalib.getPed(col,row,isdead_p,isnoisy_p)*gain;

  float vcal = adc*gain - pedestal;
  int electrons = int(vcal*ElectronADCGain_);

  return electrons;

}
