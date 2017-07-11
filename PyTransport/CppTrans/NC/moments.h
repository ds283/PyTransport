#ifndef back_H  // Prevents the class being re-definFd
#define back_H 
#include "model.h"
#include <iostream>
#include "fieldmetric.h"
#include <math.h>
#include <cmath>
#include <vector>
using namespace std;

class back 
{	
private:
	int nF;
	vector<double> f;
    fieldmetric fmet;

public:
	//constructor for background store
	back(int nFi, vector<double> f )
	{
    nF=nFi;
        f.resize(2*nF);
    }
	
    //back asscessors
	vector<double> getB()
	{
		return f;
	}
	
    double getB(int i)
	{
		return f[i];
	}
	
    //back modifiers
	void setB(int i, double value)
	{
		f[i]=value;
	}
	void setB(vector<double> y)
	{
		f=y;
	}
	
    //print back to screen
	void printB()
	{
		for(int i=0;i<2*nF;i++){std::cout << f[i] << '\t';}
		std::cout << std::endl;
	}
};
#endif 


#ifndef sigma_H  // Prevents the class being re-definFd
#define sigma_H 
#include "model.h"
#include <iostream>
#include <math.h>
using namespace std;

class sigma 
{	
private:
	int nF;
	vector<double> sig;
	double k;

public:
	//constructor for sig deep inside horizon
	sigma(int nFi, double k1, double N0, vector<double> f, vector<double> p)
	{
		nF=nFi;
		k=k1;
		
		fieldmetric fmet;
		vector<double> FMi;
		FMi = fmet.fmetric(f,p);
		
        double Hi;
		model m;
		double a = exp(N0);
		double s = m.scale(f,p,N0);
		sig.resize(2*nFi*2*nFi);
		
		for(int i=0; i<2*nFi*2*nFi;i++){sig[i]=0.;}
		Hi=m.H(f,p);
    	double ff = 0.5*Hi*Hi/k * 1./((a*Hi)*(a*Hi));
		double fp = -ff*Hi*s;
		double pp = ff*k*k/(a*a)*s*s;
		
        for(int i = 0; i<nFi; i++){
			for(int j=0; j<nFi;j++){ 
				sig[i + 2*nFi*j]=FMi[((2*nF)*(i)+j)]*ff;
			}
		}
		for(int i = nFi; i<2*nFi; i++){
			for(int j=nFi; j<2*nFi;j++){ 
				sig[i + 2*nFi*j]=FMi[((2*nF)*(i-nFi)+j-nFi)]*pp;
			}
		}
		for(int i = 0; i<nFi; i++){
			for(int j=nFi; j<2*nFi;j++){ 
				sig[i + 2*nFi*(j)]=FMi[((2*nF)*(i)+j-nFi)]*fp; 
			}
		}
		for(int i = nFi; i<2*nFi; i++){
			for(int j=0; j<nFi;j++){ 
				sig[(i) + 2*nFi*(j)]=FMi[((2*nF)*(i-nFi)+j)]*fp;
			}
		}
    }
    
    //constuctor for sig at horizon crossing (for canonical light field)
    sigma(int nFi, double N0, vector<double> f,vector<double> p)
	{
    	nF=nFi;
	    double Hi;
		model m;
        potential pot;
		fieldmetric fmet;

		//double a = exp(N0);
        sig.resize(2*nF*2*nF);
		for(int i=0; i<2*nFi*2*nFi;i++){sig[i]=0.;}
		Hi=m.H(f,p);
        vector<double> dVVi, dVi;
        double Vi ;
        Vi = pot.V(f,p);
        dVi = pot.dV(f,p);
        dVVi = pot.dVV(f,p);
        //double Hdi = m.Hdot(f,p);
    	double ff = 0.5*0.5*Hi*Hi/(3.142)/3.142;
		
        
        for(int i=0;i<2*nFi*2*nFi;i++){sig[i]=0;}
        
        for(int i = 0; i<nFi; i++){for(int j = 0; j<nFi; j++){
            if (i==j){sig[i + 2*nFi*j]=ff;
                double sum1=0;
                for(int k=0;k<nFi;k++){ sum1 = sum1 + (-dVVi[i +k*nFi]/Vi + dVi[i]*dVi[i]/Vi/Vi );}
                sig[i+nF + 2*nFi*(j)] = sum1*ff*Hi;
                sig[i+ 2*nFi*(j+nFi)] = sum1*ff*Hi;
                sig[i+nFi +2*nF*(j+nFi)] = sum1*sum1*ff*Hi*Hi;}}}
    }
    

	
	//sig asscessors
    vector<double>  getS()
	{
		return sig;
	}
	
    double getS(int i, int j)
	{
		return sig[i+j*2*nF];
	}

	//Sigma modifier
	void setS(int i, int j, double value)
	{
		sig[i+j*2*nF]=value;
	}
	
    void setS(vector<double> value)
	{
		sig=value;
	}
	
    //print sig to screen
	void printS()
	{
		for(int i=0;i<2*nF;i++){for(int j=0;j<2*nF;j++) {std::cout << sig[i+j*2*nF] << '\t';}}
		std::cout << std::endl;
	}
};
#endif 

#ifndef sigmaI_H  // Prevents the class being re-definFd
#define sigmaI_H 
#include "model.h"
#include <iostream>
#include <math.h>
using namespace std;

class sigmaI 
{	
private:
	int nF;
	vector<double> sigI;
	double k;

public:
	//constructor for sigI
	sigmaI(int nFi, double k1, double N0, vector<double> f,vector<double> p)
	{
		nF=nFi;
		fieldmetric fmet;

		vector<double> FMi;
		FMi = fmet.fmetric(f,p);
		double Hi;
        k=k1;
		model m;
		double a = exp(N0);
		double s=m.scale(f,p,N0);

        sigI.resize(2*nF*2*nF);
        for(int i=0; i<2*nFi*2*nFi;i++){sigI[i]=0.;}
		Hi=m.H(f,p);
		
		double fpI = 0.5*Hi*Hi * 1./(a*Hi)/(a*Hi)/a *s;
		
        for(int i = 0; i<nF; i++){
			for(int j = 0; j<nF; j++){
				sigI[i+(nF+j)*2*nF] = +FMi[((2*nF)*(i)+j)]*fpI;
				sigI[i+nF+(j)*2*nF] = -FMi[((2*nF)*(i)+j)]*fpI;
			}
        }	
	}
    
	//sig asscessors
	vector<double>  getSI()
	{
		return sigI;	
	}
	double getS(int i, int j)
	{
		return sigI[i+j*2*nF];
	}

	//Sigma modifier
	void setS(int i, int j, double value)
	{
		sigI[i+j*2*nF]=value;
	}
	void setS(vector<double> value)
	{
		sigI=value;
	}
	//print sig to screen 
	void printS()
	{
		for(int i=0;i<2*nF;i++){for(int j=0;j<2*nF;j++) {std::cout << sigI[i+j*2*nF] << '\t';}}
		std::cout << std::endl;
	}
};
#endif 


#ifndef alpha_H  // Prevents the class being re-definFd
#define alpha_H 
#include "model.h"
#include <iostream>
#include <math.h>
using namespace std;

class alpha
{	
private:
	int nF;
	vector<double> alp;
	double k1, k2, k3;
         
    public:
    //constructor for alp on subhorizon scales

	    alpha(int nFi, double k1, double k2, double k3, double N0, vector<double> f,vector<double> p)
    {
        vector<double>  fff, pff, fpf, ffp, ppf, fpp,pfp,ppp;
        double a;
        nF=nFi;
        model m;
        double s=m.scale(f,p,N0);
        a = exp(N0);

        fff = fffCalc(f,p, k1, k2, k3,N0);
		
        pff = pffCalc(f,p,k1,k2,k3,N0);
        fpf = pffCalc(f,p,k2,k1,k3,N0);
        ffp = pffCalc(f,p,k3,k1,k2,N0);

        ppf = ppfCalc(f,p,k1,k2,k3,N0);
        pfp = ppfCalc(f,p,k1,k3,k2,N0);
        fpp = ppfCalc(f,p,k2,k3,k1,N0);
        
        ppp = pppCalc(f,p,k1,k2,k3,N0);
            


        alp.resize(2*nF*2*nF*2*nF);
        
        for(int i=0;i<nF;i++){for(int j=0;j<nF;j++){for(int k=0;k<nF;k++){
          alp[i + j*2*nF + k*2*nF*2*nF] = fff[i + j*nF + k*nF*nF] ;
            alp[nF+i + j*2*nF + k*2*nF*2*nF] = pff[i + j*nF + k*nF*nF]/a   *s;// *a;
            alp[i + (nF+j)*2*nF + k*2*nF*2*nF] = fpf[j + i*nF + k*nF*nF]/a   *s;// *a;
            alp[i + j*2*nF + (nF+k)*2*nF*2*nF ] = ffp[k + i*nF + j*nF*nF ]/a   *s;// *a;
            alp[i + (nF+j)*2*nF + (nF+k)*2*nF*2*nF] = fpp[j + k*nF + i*nF*nF]/a/a  *s*s;// *a*a;
            alp[(nF+i) + (nF+j)*2*nF + (k)*2*nF*2*nF] = ppf[i + j*nF + k*nF*nF ]/a/a  *s*s;// *a*a;
            alp[(nF+i) + (j)*2*nF + (nF+k)*2*nF*2*nF] = pfp[i + k*nF + j*nF*nF ]/a/a   *s*s;// *a*a;
            alp[(nF+i) + (nF+j)*2*nF + (nF+k)*2*nF*2*nF ] = ppp[i + j*nF + k*nF*nF ]/a/a/a  *s*s*s;// *a*a*a;
        }}}

    }
	
    alpha(int nFi, double N0, vector<double> f,vector<double> p)
    {
        k1=0.;k2=0;k3=3;
        nF=nFi;
        alp.resize(2*nF*2*nF*2*nF);
        for(int i=0;i<2*nF*2*nF*2*nF;i++){
            alp[i]=0;
        }
		
    }
    
	
    // functions that calculate the intial conditions
    vector<double> fffCalc(vector<double> f, vector<double> p, double k1, double k2, double k3, double N0)
    {
        double  ks, K2;
        vector<double> C123, C132, C231, B123, B132, B231, AS123, AS132, AS231;
		fieldmetric fmet;

		vector<double> FMi;
		FMi = fmet.fmetric(f,p);
        model m;
        C123 = m.Ccalcd(f,p, k1, k2, k3, N0);
        C132 = m.Ccalcd(f,p, k1, k3, k2, N0);
        C231 = m.Ccalcd(f,p, k2, k3, k1, N0);

        AS123 = m.AScalcd(f, p, k1, k2, k3, N0);
        AS132 = m.AScalcd(f, p, k1, k3, k2, N0);
        AS231 = m.AScalcd(f, p, k2, k3, k1, N0);

        B123 = m.Bcalcd(f, p, k1, k2, k3, N0);
        B132 = m.Bcalcd(f, p, k1, k3, k2, N0);
        B231 = m.Bcalcd(f, p, k2, k3, k1, N0);
        
        vector<double> fff(nF*nF*nF);
        double Hi= m.H(f,p);
        
        double a =exp(N0);
        ks = k1 + k2 + k3;
        K2 = k1*k2 + k1*k3 + k2*k3;
        for(int i=0;i<nF;i++){for(int j=0;j<nF;j++){for(int k=0;k<nF;k++){
                        fff[i+j*nF+k*nF*nF] = 1./(a*a*a*a)/4. / (k1*k2*k3)/ks*(-C123[i+nF*j+nF*nF*k]*k1*k2 - C132[i+nF*k+nF*nF*j]*k1*k3 - C231[j+nF*k+nF*nF*i]*k2*k3
                                                                   + a*a*AS123[i+nF*j+nF*nF*k] + a*a*AS132[i+nF*k+nF*nF*j] + a*a*AS231[j+nF*k+nF*nF*i]
                                                                   + a*a*Hi*B123[i+nF*j+nF*nF*k]*((k1+k2)*k3/k1/k2 - K2/k1/k2)
                                                                   + a*a*Hi*B132[i+nF*k+nF*nF*j]*((k1+k3)*k1/k1/k3 - K2/k1/k3)
                                                                   + a*a*Hi*B231[j+nF*k+nF*nF*i]*((k2+k3)*k1/k2/k3 - K2/k2/k3)); 
            fff[i+j*nF+k*nF*nF] = fff[i+j*nF+k*nF*nF] + FMi[((2*nF)*(j)+k)]*1./(a*a*a*a)/4./(k1*k2*k3)/ks*f[nF+i]/2./Hi*(-k2*k2-k3*k3+k1*k1)/2.;
            fff[i+j*nF+k*nF*nF] = fff[i+j*nF+k*nF*nF] + FMi[((2*nF)*(i)+k)]*1./(a*a*a*a)/4./(k1*k2*k3)/ks*f[nF+j]/2./Hi*(-k1*k1-k3*k3+k2*k2)/2.;
            fff[i+j*nF+k*nF*nF] = fff[i+j*nF+k*nF*nF] + FMi[((2*nF)*(i)+j)]*1./(a*a*a*a)/4./(k1*k2*k3)/ks*f[nF+k]/2./Hi*(-k1*k1-k2*k2+k3*k3)/2.;
        }}}
        return fff;
    }

	
    vector<double> pffCalc(vector <double> f,vector<double> p, double k1, double k2, double k3, double N0)
    {
        double a, Hi, ks, k3s, K2;
        vector<double> pff(nF*nF*nF);
        vector<double> C123, C132, C231, B123, B132, B231, AS123, AS132, AS231;
		fieldmetric fmet;

        vector<double> FMi;
		FMi = fmet.fmetric(f,p);
		
        model m;
        
        C123 = m.Ccalcd(f, p, k1, k2, k3, N0);
        C132 = m.Ccalcd(f, p, k1, k3, k2, N0);
        C231 = m.Ccalcd(f, p, k2, k3, k1, N0);
        
        B123 = m.Bcalcd(f, p, k1, k2, k3, N0);
        B132 = m.Bcalcd(f, p, k1, k3, k2, N0);
        B231 = m.Bcalcd(f, p, k2, k3, k1, N0);
        
		AS123 = m.AScalcd(f, p, k1, k2, k3, N0);
        AS132 = m.AScalcd(f, p, k1, k3, k2, N0);
        AS231 = m.AScalcd(f, p, k2, k3, k1, N0);
        
        Hi=m.H(f,p);
        
        a=exp(N0);
        k3s = k1*k1*k1 * k2*k2*k2 * k3*k3*k3;
        ks = k1 + k2 + k3;
        K2 = k1*k2 + k1*k3 + k2*k3;
        
        for(int i=0;i<nF;i++){for(int j=0;j<nF;j++){for(int k=0;k<nF;k++){
            pff[i+j*nF+k*nF*nF] = - 1./(a*a*a)/4./k3s * Hi * (-k1*k1*(k2+k3)/ks* k1*k2*k3) * (-C123[i+nF*j+nF*nF*k]*k1*k2 - C132[i+nF*k+nF*nF*j]*k1*k3 - C231[j+nF*k+nF*nF*i]*k2*k3
                                                                                              + a*a*AS123[i+nF*j+nF*nF*k] + a*a*AS132[i+nF*k+nF*nF*j] + a*a*AS231[j+nF*k+nF*nF*i]
                                                                                              );
			pff[i+j*nF+k*nF*nF] = pff[i+j*nF+k*nF*nF] - FMi[((2*nF)*(j)+k)]*1./(a*a*a)/4./k3s * Hi * (-k1*k1*(k2+k3)/ks* k1*k2*k3) * f[nF+i]/2./Hi*(-k2*k2-k3*k3+k1*k1)/2.;
            pff[i+j*nF+k*nF*nF] = pff[i+j*nF+k*nF*nF] - FMi[((2*nF)*(i)+k)]*1./(a*a*a)/4./k3s * Hi * (-k1*k1*(k2+k3)/ks* k1*k2*k3) * f[nF+j]/2./Hi*(-k1*k1-k3*k3+k2*k2)/2.;
            pff[i+j*nF+k*nF*nF] = pff[i+j*nF+k*nF*nF] - FMi[((2*nF)*(i)+j)]*1./(a*a*a)/4./k3s * Hi * (-k1*k1*(k2+k3)/ks* k1*k2*k3) * f[nF+k]/2./Hi*(-k1*k1-k2*k2+k3*k3)/2.;
		}}}
            
        for(int i=0;i<nF;i++){for(int j=0;j<nF;j++){for(int k=0;k<nF;k++){
            pff[i+j*nF+k*nF*nF] = pff[i+j*nF+k*nF*nF] - 1./(a*a*a)/4./k3s * Hi * (-k1*k1*k2*k3/ks) * (C123[i+j*nF+k*nF*nF]*k1*k1*k2*k2*(1.+k3/ks)
                                                                                                      + C132[i+k*nF+j*nF*nF]*k1*k1*k3*k3*(1.+k2/ks)
                                                                                                      + C231[j+k*nF+i*nF*nF]*k3*k3*k2*k2*(1.+k1/ks)
                                                                                                      - a*a*AS123[i+nF*j+nF*nF*k]*(K2 - k1*k2*k3/ks)
                                                                                                      - a*a*AS132[i+nF*k+nF*nF*j]*(K2 - k1*k2*k3/ks)
                                                                                                      - a*a*AS231[j+nF*k+nF*nF*i]*(K2 - k1*k2*k3/ks)
                                                                                                      );
            pff[i+j*nF+k*nF*nF] = pff[i+j*nF+k*nF*nF] - 1./(a*a*a)/4./k3s * Hi * (-k1*k1*k2*k3/ks) * (B123[i+j*nF+k*nF*nF]/Hi*k1*k2*k3*k3
                                                                                                      + B132[i+k*nF+j*nF*nF]/Hi*k1*k3*k2*k2
                                                                                                      + B231[j+k*nF+i*nF*nF]/Hi*k2*k3*k1*k1);
			pff[i+j*nF+k*nF*nF] = pff[i+j*nF+k*nF*nF] - FMi[((2*nF)*(j)+k)]*1./(a*a*a)/4./k3s * Hi * (-k1*k1*k2*k3/ks) * f[nF+i]/2./Hi*(-1.)*(-k2*k2-k3*k3+k1*k1)/2.*(K2 +k1*k2*k3/ks);
            pff[i+j*nF+k*nF*nF] = pff[i+j*nF+k*nF*nF] - FMi[((2*nF)*(i)+k)]*1./(a*a*a)/4./k3s * Hi * (-k1*k1*k2*k3/ks) * f[nF+j]/2./Hi*(-1.)*(-k1*k1-k3*k3+k2*k2)/2.*(K2 +k1*k2*k3/ks);
            pff[i+j*nF+k*nF*nF] = pff[i+j*nF+k*nF*nF] - FMi[((2*nF)*(i)+j)]*1./(a*a*a)/4./k3s * Hi * (-k1*k1*k2*k3/ks) * f[nF+k]/2./Hi*(-1.)*(-k1*k1-k2*k2+k3*k3)/2.*(K2 +k1*k2*k3/ks);
        }}}
        return pff;
    }
    
	
    vector<double> ppfCalc(vector<double> f,vector<double> p,  double k1, double k2, double k3,double N0)
    {
        double a, H, ks, k3s;
        vector<double> ppf(nF*nF*nF);
        vector<double> C123, C132, C231, B123, B132, B231, AS123, AS132, AS231;
        model m;
        fieldmetric fmet;

        vector<double> FMi;
		FMi = fmet.fmetric(f,p);
        
        C123 = m.Ccalcd(f,p, k1, k2, k3, N0);
        C132 = m.Ccalcd(f,p, k1, k3, k2, N0);
        C231 = m.Ccalcd(f,p, k2, k3, k1, N0);
        
        B123 = m.Bcalcd(f, p, k1, k2, k3, N0);
        B132 = m.Bcalcd(f, p, k1, k3, k2, N0);
        B231 = m.Bcalcd(f, p, k2, k3, k1, N0);
        
        AS123 = m.AScalcd(f, p, k1, k2, k3, N0);
        AS132 = m.AScalcd(f, p, k1, k3, k2, N0);
        AS231 = m.AScalcd(f, p, k2, k3, k1, N0);
        
        
        H=m.H(f,p);
        
        a=exp(N0);
        k3s = k1*k1*k1 * k2*k2*k2 * k3*k3*k3;
        ks = k1 + k2 + k3;
        
        
        for(int i=0;i<nF;i++){for(int j=0;j<nF;j++){for(int k=0;k<nF;k++){
            ppf[i+j*nF+k*nF*nF] = -1./(a*a*a*a)/4./k3s * (k1*k2*k3)*(k1*k2*k3)/ks*k1*k2*(-C123[i+j*nF+k*nF*nF]*k1*k2 - C132[i+k*nF+j*nF*nF]*k1*k3 - C231[j+k*nF+i*nF*nF]*k2*k3
                                                                                         + a*a*AS123[i+nF*j+nF*nF*k] + a*a*AS132[i+nF*k+nF*nF*j] + a*a*AS231[j+nF*k+nF*nF*i]
                                                                                         + a*a*H*B123[i+nF*j+nF*nF*k]*(k1+k2)*k3/k1/k2
                                                                                         + a*a*H*B132[i+nF*k+nF*nF*j]*(k1+k3)*k2/k1/k3
                                                                                         + a*a*H*B231[j+nF*k+nF*nF*i]*(k2+k3)*k1/k2/k3
                                                                                         + k1*k1*k2*k2*a*a*H*B123[i+nF*j+nF*nF*k]*k1*k2*k3*k3
                                                                                         + k1*k1*k2*k2*a*a*H*B132[i+nF*k+nF*nF*j]*k1*k3*k2*k2
                                                                                         + k1*k1*k2*k2*a*a*H*B231[j+nF*k+nF*nF*i]*k2*k3*k1*k1);
            ppf[i+j*nF+k*nF*nF] = ppf[i+j*nF+k*nF*nF] - FMi[((2*nF)*(j)+k)]*1./(a*a*a*a)/4/k3s * (k1*k2*k3)*(k1*k2*k3)/ks*k1*k2*f[nF+i]/2./H*(-k2*k2-k3*k3+k1*k1)/2.;
            ppf[i+j*nF+k*nF*nF] = ppf[i+j*nF+k*nF*nF] - FMi[((2*nF)*(i)+k)]*1./(a*a*a*a)/4/k3s * (k1*k2*k3)*(k1*k2*k3)/ks*k1*k2*f[nF+j]/2./H*(-k1*k1-k3*k3+k2*k2)/2.;
            ppf[i+j*nF+k*nF*nF] = ppf[i+j*nF+k*nF*nF] - FMi[((2*nF)*(i)+j)]*1./(a*a*a*a)/4/k3s * (k1*k2*k3)*(k1*k2*k3)/ks*k1*k2*f[nF+k]/2./H*(-k1*k1-k2*k2+k3*k3)/2.;
            
        }}}
        return ppf;
    }
    

    vector<double> pppCalc(vector<double> f,vector<double> p,  double k1, double k2, double k3,double N0)
    {
        
        double a, H, ks, k3s, K2;
        vector<double> ppp(nF*nF*nF);
        vector<double> C123, C132, C231, B123, B132, B231, AS123, AS132, AS231;
        model m;
        fieldmetric fmet;

        vector<double> FMi;
		FMi = fmet.fmetric(f,p);
        C123 = m.Ccalcd(f,p, k1, k2, k3, N0);
        C132 = m.Ccalcd(f,p, k1, k3, k2, N0);
        C231 = m.Ccalcd(f,p, k2, k3, k1, N0);
        
        B123 = m.Bcalcd(f,p, k1, k2, k3, N0);
        B132 = m.Bcalcd(f,p, k1, k3, k2, N0);
        B231 = m.Bcalcd(f,p, k2, k3, k1, N0);
        
		AS123 = m.AScalcd(f, p, k1, k2, k3, N0);
        AS132 = m.AScalcd(f, p, k1, k3, k2, N0);
        AS231 = m.AScalcd(f, p, k2, k3, k1, N0);
        
        H=m.H(f,p);
        a=exp(N0);
        k3s = k1*k1*k1 * k2*k2*k2 * k3*k3*k3;
        ks = k1 + k2 + k3;
        K2 = k1*k2 + k1*k3 + k2*k3;
        
        for(int i=0;i<nF;i++){for(int j=0;j<nF;j++){for(int k=0;k<nF;k++){
            ppp[i+j*nF+k*nF*nF] = - 1./(a*a*a)/4./k3s * H * (k1*k1*k2*k2*k3*k3)/ks  * (C123[i+j*nF+k*nF*nF]*k1*k1*k2*k2*(1.+k3/ks)
                                                                                       + C132[i+k*nF+j*nF*nF]*k1*k1*k3*k3*(1.+k2/ks)
                                                                                       + C231[j+k*nF+i*nF*nF ]*k3*k3*k2*k2*(1.+k1/ks)
                                                                                       - a*a*AS123[i+nF*j+nF*nF*k]*(K2 - k1*k2*k3/ks)
                                                                                       - a*a*AS132[i+nF*k+nF*nF*j]*(K2 - k1*k2*k3/ks)
                                                                                       - a*a*AS231[j+nF*k+nF*nF*i]*(K2 - k1*k2*k3/ks)
                                                                                       );
            ppp[i+j*nF+k*nF*nF ] = ppp[i+j*nF+k*nF*nF] - 1./(a*a*a)/4./k3s * H * (k1*k1*k2*k2*k3*k3)/ks * (B123[i+j*nF+k*nF*nF]/H*k1*k2*k3*k3
                                                                                                           + B132[i+k*nF+j*nF*nF]/H*k1*k3*k2*k2
                                                                                                           + B231[j+k*nF+i*nF*nF]/H*k2*k3*k1*k1);
            ppp[i+j*nF+k*nF*nF] = ppp[i+j*nF+k*nF*nF] - FMi[((2*nF)*(j)+k)]*1./(a*a*a)/4./k3s * H * (k1*k1*k2*k2*k3*k3)/ks * f[nF+i]/2./H*(-1.)*(-k2*k2-k3*k3+k1*k1)/2.*(K2 +k1*k2*k3/ks);
            ppp[i+j*nF+k*nF*nF] = ppp[i+j*nF+k*nF*nF] - FMi[((2*nF)*(i)+k)]*1./(a*a*a)/4./k3s * H * (k1*k1*k2*k2*k3*k3)/ks * f[nF+j]/2./H*(-1.)*(-k1*k1-k3*k3+k2*k2)/2.*(K2 +k1*k2*k3/ks);
            ppp[i+j*nF+k*nF*nF] = ppp[i+j*nF+k*nF*nF] - FMi[((2*nF)*(i)+j)]*1./(a*a*a)/4./k3s * H * (k1*k1*k2*k2*k3*k3)/ks * f[nF+k]/2./H*(-1.)*(-k1*k1-k2*k2+k3*k3)/2.*(K2 +k1*k2*k3/ks);
            
        }}}
        return ppp;
    }

    
    
	//alpha asscessors
	vector<double> getA()
	{
		return alp;
	}
	
	double getA(int i, int j, int k)
	{
        return alp[i+j*2*nF + k*2*2*nF*nF];
	}

	//Alpha modifier
	void setA(int i, int j, int k, double value)
	{
		alp[i+j*2*nF + k*2*2*nF*nF]=value;
	}
	void setA(vector<double> value)
	{
		alp=value;
	}
	//print sig to screen 
	
    void printA()
	{
		for(int i=0;i<2*nF;i++){for(int j=0;j<2*nF;j++){for(int k=0;k<2*nF;k++) {std::cout << alp[i+j*2*nF+k*2*2*nF*nF] << '\t';}}}
		std::cout << std::endl;
	}
};
#endif 