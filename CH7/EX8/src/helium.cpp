#include "QatPlotWidgets/PlotView.h"
#include "QatPlotWidgets/setLightPalette.h"
#include "QatPlotting/PlotStream.h"

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

using namespace Genfun;
typedef std::mt19937 EngineType;

std::vector<double> integrator(double d){

  EngineType e;
  std::uniform_real_distribution<double> cosDist(-1.0, 1.0);
  std::exponential_distribution<double> expDist;
  int NEvals = 100000;

  double integral1 = 0.0;
  double integral2 = 0.0;
  double integral3 = 0.0;

  Exp exp;
  Sqrt sqrt;

  Variable R(0,2),U(1,2);
  GENFUNCTION f1 = 2 *R*R* exp(-sqrt(R*R + d*d-2*R*d*U));
  GENFUNCTION f2 = 2 * exp(-R) * (-R*R*R*R/2.0 - R*R*R - R*R/sqrt(R*R + d*d + 2*R*d*U));
  GENFUNCTION f3 = 2 * exp(-sqrt(R*R + d*d + 2*R*d*U)) * (-R*R*R*R/2.0 - R*R*R - R*R/sqrt(R*R + d*d + 2*R*d*U));
  Argument arg(2); // 2-dimensional argument
  
  for(int i=0;i<NEvals; i++) {
    arg[0] = expDist(e);
    arg[1] = cosDist(e);
    
    integral1 += f1(arg);
    integral2 += f2(arg);
    integral3 += f3(arg);
  }
  
  integral1 *= 2.0/NEvals;
  integral2 *= 2.0/NEvals;
  integral3 *= 2.0/NEvals;

  std::vector results = {integral1, integral2, integral3};
  return results;
}

double getNa(double d){
  double Na = sqrt(1/(2.0*(1.0 - integrator(d)[0])));
  return  Na;
}

double getNs(double d){
  double Ns = sqrt(1/(2.0*(1.0 + integrator(d)[0])));
  return  Ns;
}

double getEnergys(double d){
  std::vector integrals = integrator(d);
  double Ns = sqrt(1/(2.0*(1.0 + integrals[0])));

  double H11 = Ns*Ns * integrals[1];
  double H12 = Ns*Ns * integrals[2];

  double Energys = 2.0 * (H11 + H12);
  return Energys;
}

double getEnergya(double d){
  std::vector integrals = integrator(d);
  double Na = sqrt(1/(2.0*(1.0 - integrals[0])));

  double H11 = Na*Na * integrals[1];
  double H12 = Na*Na * integrals[2];

  double Energya = 2.0 * (H11 - H12);
  return Energya;
}

//=================================================

int main (int argc, char * * argv) {

  std::string usage= std::string("usage: ") + argv[0]; 
  if (argc!=1) {
    std::cout << usage << std::endl;
  }


  //===============================================
                    //Main Program
  //===============================================
  F1D NsFunction{getNs};
  F1D NaFunction{getNa};

  F1D EaFunction{getEnergya};
  F1D EsFunction{getEnergys};
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

  PlotFunction1D NaPlot{NaFunction};
  PlotFunction1D NsPlot{NsFunction};
  PlotFunction1D EaPlot{EaFunction};
  PlotFunction1D EsPlot{EsFunction};

  PRectF rectN{0.0, 6.0, 0.0, 2.0};
  PRectF rectE{0.0, 10.0, -5.0, 2.0};
  PlotView view(rectE);


  window.setCentralWidget(&view);
  
  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear()
	      << PlotStream::Center() 
	      << PlotStream::Family("Arial") 
	      << PlotStream::Size(16)
        << "Normalization Factors"
	      << PlotStream::EndP();
  
  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
         << "d (Bohr Radius)"
	       << PlotStream::EndP();
  
  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
         << "Na, Ns"
	       << PlotStream::EndP();
  
  // view.add(&NaPlot);
  // view.add(&NsPlot);
  view.add(&EsPlot);
  view.add(&EaPlot);
  window.show();
  app.exec();
  return 1;
}

