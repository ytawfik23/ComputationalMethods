#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"

#include "QatGenericFunctions/Sqrt.h"
#include "QatGenericFunctions/Cos.h"
#include "QatGenericFunctions/F1D.h"

#include "QatGenericFunctions/SimpleIntegrator.h"
#include "QatGenericFunctions/QuadratureRule.h"

#include "QatPlotting/PlotProfile.h"
#include "QatPlotting/PlotFunction1D.h"

#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <cstdlib>
#include <iostream>
#include <string>
#include <numbers>

double f(double thetaMax) {
  Genfun::GENFUNCTION cos = Genfun::Cos();
  Genfun::GENFUNCTION sqrt = Genfun::Sqrt();

  Genfun::GENFUNCTION integrand = 1/sqrt(cos-cos(thetaMax));

  Genfun::SimpleIntegrator integrator(0.0,thetaMax, Genfun::OpenTrapezoidRule(), 1000);
  double result=sqrt(2.0)*integrator(integrand)/M_PI;
  return result;

}

int main (int argc, char * * argv) {

  // Automatically generated:-------------------------:

  std::string usage= std::string("usage: ") + argv[0]; 
  if (argc!=1) {
    std::cout << usage << std::endl;
  }

  QApplication     app(argc,argv);
  
  QMainWindow window;
  QToolBar *toolBar=window.addToolBar("Tools");
  QAction  *quitAction=toolBar->addAction("Quit");
  
  quitAction->setShortcut(QKeySequence("q"));
  
  QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);
  
  PRectF rect;
  rect.setXmin(0.0);
  rect.setXmax(M_PI);
  rect.setYmin(0.0);
  rect.setYmax(2.5);
  

  PlotView view(rect);
  window.setCentralWidget(&view);
  
  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear()
	      << PlotStream::Center() 
	      << PlotStream::Family("Arial") 
	      << PlotStream::Size(16)
        << "Pendulum Period as a Function of Maximum Angle"
	      << PlotStream::EndP();
  
  
  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
         << "Theta_max"
	       << PlotStream::EndP();
  
  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
         << "T/T_0"
	       << PlotStream::EndP();
  
  
  Genfun::F1D fFunction{f};
  PlotFunction1D fPlot{fFunction};
  view.add(&fPlot);

  PlotProfile fPoints;
  std::vector<double> thetaMaxValues = {M_PI/12.0, M_PI/6.0, M_PI/3.0, M_PI/2.0, 2.0*M_PI/3.0, 5.0*M_PI/6.0, 11.0*M_PI/12.0};
  for (double thetaMax : thetaMaxValues){
    fPoints.addPoint(thetaMax, f(thetaMax));
    std::cout << "Theta_max: " << thetaMax << ", T/T_0: " << f(thetaMax) << std::endl;
  }

  PlotProfile::Properties fPointsProperties;
  fPointsProperties.pen.setColor("red");
  fPointsProperties.pen.setWidth(4);
  fPoints.setProperties(fPointsProperties);
  view.add(&fPoints);
  window.show();
  app.exec();
  return 1;
}

