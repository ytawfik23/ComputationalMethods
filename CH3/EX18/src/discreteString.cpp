#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <cstdlib>
#include <iostream>
#include <string>

#include <eigen3/Eigen/Dense>
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Cos.h"
#include "QatPlotting/PlotFunction1D.h"

int main (int argc, char * * argv) {

  // Automatically generated:-------------------------:

  
  if (argc!=2) {
    std::cout << "Usage: " << argv[0] << "Point Number" << std::endl;
  }
  int i = std::stoi(argv[1]);
  

  Eigen::MatrixXd K(100, 100);
  K.setIdentity();
  K(0, 1) = -0.5;
  K(99, 98) = -0.5;
  for(int row = 1; row < 99; row++)
  {
    K(row, row-1) = -0.5;
    K(row, row+1) = -0.5;
  }
  
  Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(K);

  Eigen::VectorXd frequencies = solver.eigenvalues().array().sqrt();
  Eigen::MatrixXd normalModes = solver.eigenvectors();

  Genfun::Variable time;
  Genfun::Cos cos;

  Genfun::GENFUNCTION x0i =  normalModes(i,0)*cos(frequencies[0] * time) + 0.0;
  Genfun::GENFUNCTION x1i =  normalModes(i,1)*cos(frequencies[1] * time) + 0.2;
  Genfun::GENFUNCTION x2i =  normalModes(i,2)*cos(frequencies[2] * time) + 0.4;
  Genfun::GENFUNCTION x3i =  normalModes(i,3)*cos(frequencies[3] * time) + 0.6;
  Genfun::GENFUNCTION x4i =  normalModes(i,4)*cos(frequencies[4] * time) + 0.8;

  PlotFunction1D px0i{x0i}, px1i{x1i}, px2i{x2i}, px3i{x3i}, px4i{x4i};
  
// Application Stuff ----------------------------------------------------------------------------:
  QApplication     app(argc,argv);
  
  QMainWindow window;
  QToolBar *toolBar=window.addToolBar("Tools");
  QAction  *quitAction=toolBar->addAction("Quit");
  
  quitAction->setShortcut(QKeySequence("q"));
  
  QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);
  //End Application stuff ------------------------------------------------------:


  PRectF rect;
  rect.setXmin(0.0);
  rect.setXmax(2*M_PI/frequencies[0]);
  rect.setYmin(-0.2);
  rect.setYmax(1.0);
  

  PlotView view(rect);
  window.setCentralWidget(&view);
  
  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear()
	      << PlotStream::Center() 
	      << PlotStream::Family("Arial") 
	      << PlotStream::Size(16)
        << "x_" << i << "(t) for First Five Eigenmodes"
	      << PlotStream::EndP();
  
  
  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
         << "omega_0 * t"
	       << PlotStream::EndP();
  
  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
         << "x_" << i << "(t)"
	       << PlotStream::EndP();
  
  view.add(&px0i);
  view.add(&px1i);
  view.add(&px2i);
  view.add(&px3i);
  view.add(&px4i);
  
  window.show();
  app.exec();
  return 1;
}

