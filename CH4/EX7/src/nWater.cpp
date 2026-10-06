#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"

#include "QatGenericFunctions/InterpolatingFunction.h"
#include "QatGenericFunctions/InterpolatingPolynomial.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatPlotting/PlotProfile.h"

#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>


int main (int argc, char * * argv) {

  // Automatically generated:-------------------------:

  std::string usage= std::string("usage: ") + argv[0]; 
  if (argc!=1) {
    std::cout << usage << std::endl;
  }

  std::vector<double> wavelength = {200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000};

  std::vector<double> indexN = {1.396, 1.362, 1.349, 1.343, 1.339, 1.337, 1.335, 1.333, 1.332, 1.331, 1.331, 1.330, 1.329, 1.329, 1.328, 1.327, 1.327};


  Genfun::InterpolatingFunction interpolation(Genfun::CUBIC_SPLINE);
  PlotProfile plotpoints;

  for(int i=0; i<wavelength.size(); i++) {
    interpolation.addPoint(wavelength[i], indexN[i]);
    plotpoints.addPoint(wavelength[i], indexN[i]);
  }

  PlotFunction1D nWaterPlot{interpolation};


  QApplication     app(argc,argv);
  
  QMainWindow window;
  QToolBar *toolBar=window.addToolBar("Tools");
  QAction  *quitAction=toolBar->addAction("Quit");
  
  quitAction->setShortcut(QKeySequence("q"));
  
  QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);
  
  PRectF rect;
  rect.setXmin(200.0);
  rect.setXmax(1000.0);
  rect.setYmin(1.327);
  rect.setYmax(1.396);
  

  PlotView view(rect);
  window.setCentralWidget(&view);
  
  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear()
	      << PlotStream::Center() 
	      << PlotStream::Family("Arial") 
	      << PlotStream::Size(16)
        << "n(lambda) Interpolation"
	      << PlotStream::EndP();
  
  
  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
         << "wavelength (nm)"
	       << PlotStream::EndP();
  
  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
         << "index of refraction"
	       << PlotStream::EndP();
  
  PlotProfile::Properties plotpointsProperties;
  plotpointsProperties.pen.setColor("black");
  plotpointsProperties.pen.setWidth(4);
  plotpoints.setProperties(plotpointsProperties);

  
  view.add(&nWaterPlot);
  view.add(&plotpoints);
  window.show();
  app.exec();
  return 1;
}

