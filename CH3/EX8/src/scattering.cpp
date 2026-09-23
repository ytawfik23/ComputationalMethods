#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <cstdlib>
#include <iostream>
#include <string>

#include "QatGenericFunctions/Sin.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatGenericFunctions/F1D.h"

#include <numbers>

#include <eigen3/Eigen/Dense>
#include <iostream>

#include <iostream>
#include <eigen3/Eigen/Dense>
#include <complex>

Eigen::VectorXcd coefficients = Eigen::VectorXcd::Zero(8);
double ka = 2.0* M_PI/0.6;
double v = 1.5;

double potential(double x){
  double pot{0};
  if      (x <= -2) {pot = 0;}
  else if (x <= -1) {pot = v;}
  else if (x <=  1) {pot = 2*v;}
  else if (x <=  2) {pot = v;}
  else              {pot = 0;}
  return pot;
}

Eigen::VectorXcd getCoefficients(double kaa) {
    std::complex<double> I{0.0, 1.0};
    std::complex<double> n1 = sqrt(std::complex(1.0 - v));
    std::complex<double> n2 = sqrt(std::complex(1.0 - 2*v));

    Eigen::MatrixXcd S 
    {
    { -std::exp(2.0*I*kaa),         std::exp(-2.0*I*n1*kaa),    std::exp(2.0*I*n1*kaa),      0.0,                        0.0,                      0.0,                        0.0,                       0.0 },
    {  std::exp(2.0*I*kaa),         n1*std::exp(-2.0*I*n1*kaa), -n1*std::exp(2.0*I*n1*kaa),  0.0,                        0.0,                      0.0,                        0.0,                       0.0 },
    {  0.0,                        std::exp(-I*n1*kaa),        std::exp(I*n1*kaa),         -std::exp(-I*n2*kaa),        -std::exp(I*n2*kaa),        0.0,                        0.0,                       0.0 },
    {  0.0,                        n1*std::exp(-I*n1*kaa),     -n1*std::exp(I*n1*kaa),     -n2*std::exp(-I*n2*kaa),      n2*std::exp(I*n2*kaa),      0.0,                        0.0,                       0.0 },
    {  0.0,                        0.0,                       0.0,                        std::exp(I*n2*kaa),         std::exp(-I*n2*kaa),       -std::exp(I*n1*kaa),         -std::exp(-I*n1*kaa),        0.0 },
    {  0.0,                        0.0,                       0.0,                        n2*std::exp(I*n2*kaa),     -n2*std::exp(-I*n2*kaa),     -n1*std::exp(I*n1*kaa),       n1*std::exp(-I*n1*kaa),      0.0 },
    {  0.0,                        0.0,                       0.0,                        0.0,                        0.0,                       std::exp(2.0*I*n1*kaa),      std::exp(-2.0*I*n1*kaa),   -std::exp(2.0*I*kaa) },
    {  0.0,                        0.0,                       0.0,                        0.0,                        0.0,                       n1*std::exp(2.0*I*n1*kaa),  -n1*std::exp(-2.0*I*n1*kaa), -std::exp(2.0*I*kaa) }
    };

    Eigen::VectorXcd input {{std::exp(-2.0*I*kaa)}, {std::exp(-2.0*I*kaa)}, {0.0}, {0.0}, {0.0}, {0.0}, {0.0}, {0.0}};

    Eigen::VectorXcd coefficients ((S.inverse())*input);
    return coefficients;
}

double PsiX(double x)
{
  std::complex<double> I{0.0, 1.0};
  std::complex<double> psix{0};
  std::complex<double> n1 = sqrt(std::complex(1.0 - v));
  std::complex<double> n2 = sqrt(std::complex(1.0 - 2*v));

  if      (x <= -2.0) {psix = 1.0             * std::exp(I*ka*x)    + coefficients[0] * std::exp(-I*ka*x);}
  else if (x <= -1.0) {psix = coefficients[1] * std::exp(I*n1*ka*x) + coefficients[2] * std::exp(-I*n1*ka*x);}
  else if (x <=  1.0) {psix = coefficients[3] * std::exp(I*n2*ka*x) + coefficients[4] * std::exp(-I*n2*ka*x);}
  else if (x <=  2.0) {psix = coefficients[5] * std::exp(I*n1*ka*x) + coefficients[6] * std::exp(-I*n1*ka*x);}
  else              {psix = coefficients[7] * std::exp(I*ka*x);}
  return psix.real();
}

PlotFunction1D plotPsi() {

  Genfun::F1D PsiXFunction{PsiX};
  PlotFunction1D PlotPsiX{PsiXFunction};
  return PlotPsiX;

}


int main (int argc, char * * argv) {

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
  rect.setXmin(-4.0);
  rect.setXmax(4.0);
  rect.setYmin(-2.0);
  rect.setYmax(3.0);
  

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
         << "x/a"
	       << PlotStream::EndP();
  
  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
         << "Re[psi(x)]"
	       << PlotStream::EndP();
  
  coefficients = getCoefficients(ka);
  
  PlotFunction1D pPsi = plotPsi();
  view.add(&pPsi);
  
  Genfun::F1D PotentialFunction{potential};
  PlotFunction1D plotPotential{PotentialFunction};
  view.add(&plotPotential);
  
  window.show();
  app.exec();

  return 0;
}

