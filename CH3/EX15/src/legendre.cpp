#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <cstdlib>
#include <iostream>
#include <string>

#include "QatGenericFunctions/LegendrePolynomial.h"
#include "QatGenericFunctions/RootFinder.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatPlotting/PlotProfile.h"


int main (int argc, char * * argv) {

  // Automatically generated:-------------------------:

  std::string usage= std::string("usage: ") + argv[0]; 
  if (argc!=2) {
    std::cout << usage << std::endl;
  }

  int l = std::stoi(argv[1]);

  QApplication     app(argc,argv);
  
  QMainWindow window;
  QToolBar *toolBar=window.addToolBar("Tools");
  QAction  *quitAction=toolBar->addAction("Quit");
  
  quitAction->setShortcut(QKeySequence("q"));
  
  QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);
  
  PRectF rect{-1.0, 1.0, -1.0, 1.0};
  

  PlotView view(rect);
  window.setCentralWidget(&view);
  
  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear()
	      << PlotStream::Center() 
	      << PlotStream::Family("Arial") 
	      << PlotStream::Size(16)
	      << PlotStream::EndP();
  
  
  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
	       << PlotStream::EndP();
  
  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
	       << PlotStream::EndP();
  
  Genfun::GENFUNCTION legendre = Genfun::LegendrePolynomial(l);
  Genfun::NewtonRaphson solver(legendre, 1.0E-10, 100);
  
  solver.lowerBound() = -1.0;
  solver.upperBound() = 1.0;

  PlotProfile legendreZeros{};

  double initialGuess{0};
  for(int m=1; m<=l; m++){

    initialGuess = (1.0-1/(8.0*l*l)+1.0/(8.0*l*l*l))*std::cos(M_PI*(4.0*m-1.0)/(4.0*l+2));
    double root = solver.root(initialGuess);
    std::cout  << root << std::endl;
    legendreZeros.addPoint(root, 0.0);

  }
  
  
  PlotFunction1D plotLegendre{legendre};
  
  view.add(&plotLegendre);
  view.add(&legendreZeros);

  window.show();
  app.exec();
  return 1;
}

