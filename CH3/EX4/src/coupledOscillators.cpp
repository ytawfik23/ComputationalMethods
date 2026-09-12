#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatGenericFunctions/F1D.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cmath>
#include <eigen3/Eigen/Dense>
#include <complex>
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Cos.h"



int main (int argc, char * * argv) {
// *****************************************************************************

    Eigen::VectorXd initialPositions {{-2.0},{3.0},{0.0},{-3.0},{2.0}};
    Eigen::VectorXd coefficients = Eigen::VectorXd::Zero(5);


    Eigen::MatrixXd Mhalf {
    {sqrt(1.0), 0.0, 0.0, 0.0, 0.0},
    {0.0, sqrt(3.0), 0.0, 0.0, 0.0},
    {0.0, 0.0, sqrt(2.0), 0.0, 0.0},
    {0.0, 0.0, 0.0, sqrt(1.0), 0.0},
    {0.0, 0.0, 0.0, 0.0, sqrt(2.0)}
    };
    
    Eigen::MatrixXd K {
    { 1.0, -1.0,  0.0,  0.0,  0.0},
    {-1.0,  3.0, -2.0,  0.0,  0.0},
    { 0.0, -2.0,  4.0, -2.0,  0.0},
    { 0.0,  0.0, -2.0,  3.0, -1.0},
    { 0.0,  0.0,  0.0, -1.0,  1.0},
    };

    Eigen::MatrixXd Omega2 = (Mhalf.inverse())*K*(Mhalf.inverse());

    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> s(Omega2);

    Eigen::VectorXd frequencies = s.eigenvalues().array().sqrt();
    Eigen::MatrixXd normalModesY = s.eigenvectors();
    Eigen::MatrixXd normalModesX = Mhalf.inverse()*normalModesY;
    coefficients = initialPositions.transpose() * Mhalf * Mhalf * normalModesX;


    Genfun::Variable time;
    Genfun::Cos cos;
    Genfun::GENFUNCTION x0 =  coefficients[0]*normalModesX(0,1)*cos(frequencies[0] * time)
                             +coefficients[1]*normalModesX(0,1)*cos(frequencies[1] * time)
                             +coefficients[2]*normalModesX(0,2)*cos(frequencies[2] * time)
                             +coefficients[3]*normalModesX(0,3)*cos(frequencies[3] * time)
                             +coefficients[4]*normalModesX(0,4)*cos(frequencies[4] * time)+0.0;
    
    Genfun::GENFUNCTION x1 =  coefficients[0]*normalModesX(1,1)*cos(frequencies[0] * time)
                             +coefficients[1]*normalModesX(1,1)*cos(frequencies[1] * time)
                             +coefficients[2]*normalModesX(1,2)*cos(frequencies[2] * time)
                             +coefficients[3]*normalModesX(1,3)*cos(frequencies[3] * time)
                             +coefficients[4]*normalModesX(1,4)*cos(frequencies[4] * time)+10.0;
    
    Genfun::GENFUNCTION x2 =  coefficients[0]*normalModesX(2,1)*cos(frequencies[0] * time)
                             +coefficients[1]*normalModesX(2,1)*cos(frequencies[1] * time)
                             +coefficients[2]*normalModesX(2,2)*cos(frequencies[2] * time)
                             +coefficients[3]*normalModesX(2,3)*cos(frequencies[3] * time)
                             +coefficients[4]*normalModesX(2,4)*cos(frequencies[4] * time)+20.0;
    
    Genfun::GENFUNCTION x3 =  coefficients[0]*normalModesX(3,1)*cos(frequencies[0] * time)
                             +coefficients[1]*normalModesX(3,1)*cos(frequencies[1] * time)
                             +coefficients[2]*normalModesX(3,2)*cos(frequencies[2] * time)
                             +coefficients[3]*normalModesX(3,3)*cos(frequencies[3] * time)
                             +coefficients[4]*normalModesX(3,4)*cos(frequencies[4] * time)+30.0;
    
    Genfun::GENFUNCTION x4 =  coefficients[0]*normalModesX(4,1)*cos(frequencies[0] * time)
                             +coefficients[1]*normalModesX(4,1)*cos(frequencies[1] * time)
                             +coefficients[2]*normalModesX(4,2)*cos(frequencies[2] * time)
                             +coefficients[3]*normalModesX(4,3)*cos(frequencies[3] * time)
                             +coefficients[4]*normalModesX(4,4)*cos(frequencies[4] * time)+40.0;
                        
      
//**********************************************************************
  // This is example code demonstrating how to visualize functions
  // using the QAT libraries.

  // It plots f(x) where f(x) is a function defined just above:


  // This part is "boilerplate" from the Qt package.  It sets up a window
  // which is part of the graphical user interface.  It adds a toolbar
  // with a "Quit" button, and the button has a keyboard accerator: the
  // q key can be pressed to quit the application. Interested users
  // can consult the Qt documentation for more information. 
  
  QApplication     app(argc,argv);
  QMainWindow window;
  QToolBar *toolBar=window.addToolBar("Tools");
  QAction  *quitAction=toolBar->addAction("Quit");
  quitAction->setShortcut(QKeySequence("q"));
  QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);

  // The following code sets the scale for a linear plot:
  PRectF rect;
  rect.setXmin(0.0);
  rect.setXmax( 20.0);
  rect.setYmin(-10.0);
  rect.setYmax( 50.0);
  
  // Now create a "PlotView" and put it in the window.  This is a set of
  // axes onto which one can plot functions and draw other objects. 
  PlotView view(rect);
  window.setCentralWidget(&view);
  
  // Here we make plots out of our function, first by adapting to an F1D
  // and then by adapting to a PlotFunction1D:
  
  PlotFunction1D px0=x0;
  {
    // Set plot properties here: thick, dark red
    PlotFunction1D::Properties prop;
    prop.pen.setWidth(3);
    px0.setProperties(prop);
  };
  
  PlotFunction1D px1=x1;
  {
    // Set plot properties here: thick, dark red
    PlotFunction1D::Properties prop;
    prop.pen.setWidth(3);
    px1.setProperties(prop);
  };
  
  PlotFunction1D px2=x2;
  {
    // Set plot properties here: thick, dark red
    PlotFunction1D::Properties prop;
    prop.pen.setWidth(3);
    px2.setProperties(prop);
  }
  
  PlotFunction1D px3=x3;
  {
    // Set plot properties here: thick, dark red
    PlotFunction1D::Properties prop;
    prop.pen.setWidth(3);
    px3.setProperties(prop);
  }
  
  PlotFunction1D px4=x4;
  {
    // Set plot properties here: thick, dark red
    PlotFunction1D::Properties prop;
    prop.pen.setWidth(3);
    px4.setProperties(prop);
  }
  
  // And we add them to the plotter:
   view.add(&px0);
   view.add(&px1);
   view.add(&px2);
   view.add(&px3);
   view.add(&px4);
  
  // Give the plot a title:
  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear()
	      << PlotStream::Center() 
	      << PlotStream::Family("Sans Serif") 
	      << PlotStream::Size(16)
	      << " Coupled Harmonic Oscillator"
	      << PlotStream::Normal()
	      << PlotStream::EndP();
  
  // Label the x-axis
  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Sans Serif")
	       << PlotStream::Size(16)
	       << "omega t (radians)"
	       << PlotStream::EndP();

  // Label the y-axis.
  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Sans Serif")
	       << PlotStream::Size(16)
	       << "Particle Positions (cm)"
	       << PlotStream::EndP();
  // ... add plots, set labels, etc. ...


  // Show the window and start user interaction:
  window.show();
  app.exec();
  
  view.save();   
  return 1;
}
