#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"

#include "QatGenericFunctions/SimpleIntegrator.h"
#include "QatGenericFunctions/QuadratureRule.h"
#include "QatGenericFunctions/Tanh.h"
#include "QatGenericFunctions/ATanh.h"
#include "QatGenericFunctions/Sqrt.h"

#include "QatPlotting/PlotProfile.h"

#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <cstdlib>
#include <iostream>
#include <string>
#include <numbers>

std::unique_ptr<Genfun::QuadratureRule> getRule(std::string name) {
  std::transform(name.begin(), name.end(), name.begin(), ::tolower);
 
  if (name == "trapezoid")      return std::make_unique<Genfun::TrapezoidRule>();
  if (name == "opentrapezoid")  return std::make_unique<Genfun::OpenTrapezoidRule>();
  if (name == "midpoint")       return std::make_unique<Genfun::MidpointRule>();
  if (name == "simpson")        return std::make_unique<Genfun::SimpsonsRule>();
 
  std::cerr << "Unknown rule \"" << name << "\".\n"
            << "Valid options: trapezoid, opentrapezoid, midpoint, simpson\n";
  return nullptr;
}
 
int main (int argc, char * * argv) {
 
  std::string usage = std::string("usage: ") + argv[0]
      + " <rule: trapezoid|opentrapezoid|midpoint|simpson>";
 
  if (argc != 2) {
    std::cout << usage << std::endl;
    return 1;
  }
 
  std::unique_ptr<Genfun::QuadratureRule> rule = getRule(argv[1]);
  if (!rule) {
    std::cout << usage << std::endl;
    return 1;
  }
  std::cout << "Using quadrature rule: " << argv[1] << std::endl;
 

  // Automatically generated:-------------------------:

  Genfun::GENFUNCTION tanh = Genfun::Tanh();
  Genfun::GENFUNCTION Atan = Genfun::ATanh();
  Genfun::GENFUNCTION sqrt = Genfun::Sqrt();
  Genfun::GENFUNCTION SCoth = Genfun::Sqrt() (1/Genfun::Tanh());

  double analyticalResultTan = std::log(std::cosh(1.0));
  double analyticalResultSCoth = M_PI / 2.0 + std::atanh(std::sqrt(std::tanh(1.0))) - std::atan(std::sqrt(1.0 / std::tanh(1.0)));
  
  std::vector<double> intervals = {2, 4, 8, 16, 32, 64, 128};
  int nIntervals = intervals.size();

  std::vector<double> resultsTan(nIntervals);
  std::vector<double> errorsTan(nIntervals);

  std::vector<double> resultsSCoth(nIntervals);
  std::vector<double> errorsSCoth(nIntervals);

  for (int i=0; i<nIntervals;i++){
    Genfun::SimpleIntegrator integrator(0.0,1.0, *rule, intervals[i]);
    resultsTan[i] = integrator(tanh);
    errorsTan[i] = analyticalResultTan - resultsTan[i];

    resultsSCoth[i] = integrator(SCoth);
    errorsSCoth[i] = analyticalResultSCoth - resultsSCoth[i];
  }

  PlotProfile errorProfileTan, errorProfileSCoth;
  for(int i=0; i<nIntervals; i++){
    errorProfileTan.addPoint(1/intervals[i], std::abs(errorsTan[i]));
   errorProfileSCoth.addPoint(1/intervals[i], std::abs(errorsSCoth[i]));
  }

  QApplication     app(argc,argv);
  
  QMainWindow window;
  QToolBar *toolBar=window.addToolBar("Tools");
  QAction  *quitAction=toolBar->addAction("Quit");
  
  quitAction->setShortcut(QKeySequence("q"));
  
  QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);
  
  PRectF rect;
  rect.setXmin(0.0);
  rect.setXmax(0.5);
  rect.setYmin(1e-7);
  rect.setYmax(0.5);
  

  PlotView view(rect);
  view.setLogY(true);

  window.setCentralWidget(&view);
  
  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear()
	      << PlotStream::Center() 
	      << PlotStream::Family("Arial") 
	      << PlotStream::Size(16)
        << "Integral Convergence"
	      << PlotStream::EndP();
  
  
  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
         << "1/n"
	       << PlotStream::EndP();
  
  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
         << "Error"
	       << PlotStream::EndP();
  
  PlotProfile::Properties errorProfileTanProperties;
  errorProfileTanProperties.pen.setColor("blue");
  errorProfileTanProperties.pen.setWidth(4);
  errorProfileTan.setProperties(errorProfileTanProperties);

  PlotProfile::Properties errorProfileSCothProperties;
  errorProfileSCothProperties.pen.setColor("red");
  errorProfileSCothProperties.pen.setWidth(4);
  errorProfileSCoth.setProperties(errorProfileSCothProperties);

  view.add(&errorProfileTan);
  view.add(&errorProfileSCoth);

  for (int i=0; i<nIntervals; i++){
    std::cout << "n = " << intervals[i] << ", error tanh = " << errorsTan[i] << ", error sqrt(coth) = " << errorsSCoth[i] << std::endl;
  }

  window.show();
  app.exec();
  return 1;
}

