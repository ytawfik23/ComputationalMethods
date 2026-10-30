#include "QatPlotWidgets/PlotView.h"
#include "QatPlotWidgets/setLightPalette.h"
#include "QatPlotting/PlotStream.h"

#include "QatDataAnalysis/Hist1D.h"
#include "QatPlotting/PlotHist1D.h"

#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Exp.h"
#include "QatGenericFunctions/Sqrt.h"
#include "QatGenericFunctions/Argument.h"
#include "QatGenericFunctions/F1D.h"

#include "QatGenericFunctions/Cos.h"
#include "QatGenericFunctions/Sin.h"

#include "QatPlotting/PlotFunction1D.h"

#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <cstdlib>
#include <iostream>
#include <string>
#include <random>
#include <cmath>

typedef std::mt19937 EngineType;

int main (int argc, char * * argv) {

  std::string usage= std::string("usage: ") + argv[0]; 
  if (argc!=1) {
    std::cout << usage << std::endl;
  }


  //===============================================
                    //Main Program
  //===============================================
  EngineType e;
  std::uniform_real_distribution<double> xDistribution(0.0, 1.0);
  int NSamples = 1000000;
  int NBins = 100;

  double xmin = 1.0;
  double xmax = 10.0;
  double ymin = xmin;
  double ymax = xmax;



  double BinSpacing = (ymax-ymin) / NBins;

  Hist1D hy("Y", NBins, ymin, ymax);

  for(int i=0; i<NSamples; i++){

    double x = xDistribution(e);

    double y = xmin * pow(xmax/xmin,x);
    hy.accumulate(y);

  }

  Genfun::Variable Y;
  Genfun::GENFUNCTION rho = NSamples * BinSpacing * (1.0/Y) * (1.0/log(xmax/xmin));

  //===============================================
                    //Windowing
  //===============================================

  QApplication     app(argc,argv);
  setLightPalette();
  
  QMainWindow window;
  QToolBar *toolBar=window.addToolBar("Tools");
  QAction  *quitAction=toolBar->addAction("Quit");
  
  quitAction->setShortcut(QKeySequence("q"));
  
  QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);
  
  //===============================================
                    //Plotting
  //===============================================  

  
  PlotHist1D pH=hy;
  PlotView view(pH.rectHint());
  PlotFunction1D prho{rho};

  view.add(&prho);
  view.add(&pH);

  view.yAxisFont().setPointSize(12);
  window.setCentralWidget(&view);

  std::string title = "Direct Sampling 1";
  
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
	       << "y" 
	       << PlotStream::EndP();
  
  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Sans Serif")
	       << PlotStream::Size(16)
          << "ρ"
          << "(y)"
	       << PlotStream::EndP();
  
  
  view.show();
  window.show();
  app.exec();
  return 1;
}