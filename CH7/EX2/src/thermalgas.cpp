#include "QatDataAnalysis/Hist1D.h"
#include "QatPlotting/PlotHist1D.h"

#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Exp.h"
#include "QatPlotting/PlotFunction1D.h"

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

typedef std::mt19937 EngineType;

int main (int argc, char * * argv) {

  std::string usage= std::string("usage: ") + argv[0]; 
  if (argc!=1) {
    std::cout << usage << std::endl;
  }

  //===============================================
                    //Main Program
  //===============================================

  // Histogram generation
  EngineType e;
  std::gamma_distribution rx(1.5, 2.0);
  int NumBins = 1000;
  double umin = 0.0;
  double umax = 5.0;
  double BinSpacing = (umax - umin)/NumBins;

  Hist1D hu ("U", NumBins, umin, umax);
  int NumSamples = 1000000;
  for(int i=0; i<NumSamples; i++){
    double x = rx(e);
    double u = sqrt(x);
    hu.accumulate(u);
  }

  // Distribution Function
  Genfun::Variable U;
  Genfun::GENFUNCTION rho = NumSamples* BinSpacing * sqrt(2.0/M_PI) * U*U * Genfun::Exp()(-U*U/2.0);

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

  PlotHist1D pH=hu;
  PlotView view(pH.rectHint());

  PlotFunction1D prho{rho};

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