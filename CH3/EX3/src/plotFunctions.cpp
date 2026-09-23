#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>

#include "QatPlotting/PlotFunction1D.h"
#include "QatGenericFunctions/F1D.h"
#include "QatGenericFunctions/Sin.h"
#include "QatGenericFunctions/Cos.h"
#include "QatGenericFunctions/Exp.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/AssociatedLegendre.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <numbers>
#include <vector>

using namespace Genfun;

double fF(double x) {return x*x*x;}


int main (int argc, char * * argv) {
  // IO stuff:--------------------------------------------------:

  std::string usage= std::string("Usage: ") + argv[0]; 
  if (argc!=2) {
    std::cout << usage << " <Fucntion Choice>"  << std::endl;
    return 1;
  }
  // Window Stuff:----------------------------------------------:
  QApplication     app(argc,argv);
  QMainWindow window;
  QToolBar *toolBar=window.addToolBar("Tools");
  QAction  *quitAction=toolBar->addAction("Quit");
  
  quitAction->setShortcut(QKeySequence("q"));
  
  QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);

// Generate the Functions:---------------------------------------:
 Variable X;

 GENFUNCTION A = Sin();

 GENFUNCTION B = Sin()(5*X);

 GENFUNCTION C = Sin()(X*X);

 GENFUNCTION D = Exp()(-X)*Sin();

 GENFUNCTION E = AssociatedLegendre(3, 2);

 GENFUNCTION F = F1D(fF);

 GENFUNCTION G = F.prime();

// Initialize the Plottable :--------------:

 PlotFunction1D plot{A};
 
// Setting Up the Plot:--------------------------------------:

  PRectF rect;
  
// Pick the Plot and Add it to the Window:-----------------------:

switch(std::stoi(argv[1])){
    case 1:
      rect.setXmin(0.0);rect.setXmax(2*M_PI);
      rect.setYmin(-1.1);rect.setYmax(1.1);
      plot = PlotFunction1D(A);
      break;
    case 2:
      rect.setXmin(0.0);rect.setXmax(2*M_PI);
      rect.setYmin(-1.1);rect.setYmax(1.1);
      plot = PlotFunction1D(B);
      break;
    case 3:
      rect.setXmin(0.0);rect.setXmax(2*M_PI);
      rect.setYmin(-1.1);rect.setYmax(1.1);
      plot = PlotFunction1D(C);
      break;
    case 4:
      rect.setXmin(0.0);rect.setXmax(2*M_PI);
      rect.setYmin(-0.5);rect.setYmax(0.5);
      plot = PlotFunction1D(D);
      break;
    case 5:
      rect.setXmin(-1.0);rect.setXmax(1.0);
      rect.setYmin(-6.0);rect.setYmax(6.0);
      plot = PlotFunction1D(E);
      break;
    case 6:
      rect.setXmin(0.0);rect.setXmax(1.0);
      rect.setYmin(0.0);rect.setYmax(1.0);
      plot = PlotFunction1D(F);
      break;
    case 7:
      rect.setXmin(0.0);rect.setXmax(1.0);
      rect.setYmin(0.0);rect.setYmax(1.0);
      plot = PlotFunction1D(G);
      break;
}

  PlotView view(rect);
  PlotFunction1DProperties properties;
  properties.pen.setWidth(3);
  plot.setProperties(properties);
  view.add(&plot);
  window.setCentralWidget(&view);
  
  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Family("Arial")
	      << "Problem 1"
	      << PlotStream::EndP();
  
  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Family("Arial")
	       << "x"
	       << PlotStream::EndP();
  
  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Family("Arial")
	       << "f(x)" << PlotStream::EndP();
 
  view.save("plot.png");
 
  window.show();
  app.exec();
  return 1;
}

