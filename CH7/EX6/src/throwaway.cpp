#include "QatDataAnalysis/Hist1D.h"
#include "QatPlotting/PlotHist1D.h"

#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Exp.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatGenericFunctions/LegendrePolynomial.h"

#include "QatPlotWidgets/PlotView.h"
#include "QatPlotWidgets/setLightPalette.h"
#include "QatPlotting/PlotStream.h"

#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>

#include <cstdlib>
#include <iostream>
#include <string>
#include <random>
using namespace Genfun;
typedef std::mt19937 EngineType;

int main (int argc, char * * argv) {

  std::string usage= std::string("usage: ") + argv[0]; 
  if (argc!=1) {
    std::cout << usage << std::endl;
  }

  //===============================================
                    //Main Program
  //===============================================

  //Probability Distribution Function
  Genfun::GENFUNCTION rho = (sqrt(3.0/8.0)*LegendrePolynomial(1)-sqrt(7.0/6.0)*LegendrePolynomial(3)
                            +sqrt(11.0/24.0)*LegendrePolynomial(5)-sqrt(15.0/6.0)*LegendrePolynomial(7))
                            *(sqrt(3.0/8.0)*LegendrePolynomial(1)-sqrt(7.0/6.0)*LegendrePolynomial(3)
                            +sqrt(11.0/24.0)*LegendrePolynomial(5)-sqrt(15.0/6.0)*LegendrePolynomial(7));

  // Histogram generation
  EngineType e;
  std::uniform_real_distribution<double> rx(-1.0, 1.0);
  std::uniform_real_distribution<double> ry(0.0, rho(1));
  
  int NumBins = 100;
  double umin = -1.0;
  double umax = 1.0;
  double BinSpacing = (umax - umin)/NumBins;
  int NAccepted=0;

  Hist1D hx ("X", NumBins, umin, umax);
  int NumSamples = 1000000;
  for(int i=0; i<NumSamples; i++){
    double x = rx(e);
    double y = ry(e);
    
    if(y < rho(x)) {hx.accumulate(x);NAccepted+=1;}
  }

  // Distribution Function
  Genfun::GENFUNCTION rhoN = NAccepted* BinSpacing * rho;

  //===============================================
                    //Windowing
  //===============================================

  QApplication     app(argc,argv);
  
  QMainWindow window;
  QToolBar *toolBar=window.addToolBar("Tools");
  QAction  *quitAction=toolBar->addAction("Quit");
  
  quitAction->setShortcut(QKeySequence("q"));
  
  QObject::connect(quitAction, SIGNAL(triggered()), &app, SLOT(quit()));
  

  //===============================================
                    //Plotting
  //===============================================

  PlotHist1D pH=hx;
  PlotView view(pH.rectHint());

  PlotFunction1D prho{rhoN};

  view.add(&prho);
  view.add(&pH);

  view.yAxisFont().setPointSize(12);
  window.setCentralWidget(&view);

  std::string title = "Thermal Distribution";
  
  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear()
	      << PlotStream::Center() 
	      << PlotStream::Family("Sans Serif") 
	      << PlotStream::Size(16)
	      << title
	      << PlotStream::EndP();
  
  
  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Sans Serif")
	       << PlotStream::Size(16)
	       << "u" 
	       << PlotStream::EndP();
  
  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Sans Serif")
	       << PlotStream::Size(16)
	       << "ρ"
	       << "(u)"
	       << PlotStream::EndP();
  
  
  view.show();
  window.show();
  app.exec();
  return 1;
}