#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
	double Lx = 1.0, Ly = 1.0, dx = 0.005, dy = 0.005, t = 100.0, dt = 0.001, oz = 1.9, pr = 0.7, ra = 100000, eps = 0.00001, maks, A, B, C, F, x, y,Nus, tt=0.0,
	 **teta, **omega, **U, **V,**psyn, **psy;
	int Nx = Lx / dx, Ny = Ly / dy, Nt = t / dt;
	
	psy = new double*[Nx];
	psyn = new double*[Nx];
	teta = new double*[Nx];
	omega = new double*[Nx];
	U = new double*[Nx];
	V = new double*[Nx];
	for (int i = 0; i <= Nx; i++)
	{
		psy[i] = new double[Ny];
		psyn[i] = new double[Ny];
		teta[i] = new double[Ny];
		omega[i] = new double[Ny];
		U[i] = new double[Ny];
		V[i] = new double[Ny];
	}
	for (int i = 0; i <= Nx; i++)
		for (int j = 0; j <= Ny; j++)
		{
			
			psy[i][j] = 0.0;
			omega[i][j] = 0.0;
			teta[i][j] = 0.50;

		}
	double *alpha = new double[Nx];
	double *beta = new double[Nx];
	double *Nu = new double[Ny];
	
	
	ofstream fout4("Nu200.txt");
	
    
	for (int ts = 1; ts <= Nt; ts++)
	{ 
		maks = 1.0 + eps;
		while(abs(maks)>eps) {

			
			for (int i = 0; i <= Nx; i++)
			{
				for (int j = 0; j <= Ny; j++)
				{
					psyn[i][j] = psy[i][j];
				}
			}
			for (int i = 1; i <= Nx - 1; i++)
			{
				for (int j = 1; j <= Ny - 1; j++)
				{
					psy[i][j] = (dx*dx*dy*dy / (2.0*dx*dx + 2.0*dy*dy))*(omega[i][j] + ((psy[i + 1][j] + psy[i - 1][j]) / (dx*dx)) + ((psy[i][j + 1] + psy[i][j - 1]) / (dy*dy)));
					psy[i][j] = psyn[i][j] + oz*(psy[i][j] - psyn[i][j]);
				}
			}
			maks = 0.0;
			for (int i = 0; i <= Nx; i++)
			{
				
				for (int j = 0; j <= Ny; j++)
				{
					if ((abs(psy[i][j] - psyn[i][j])) > maks)  
						maks = abs(psy[i][j] - psyn[i][j]);	
					
				}
			}
			
			} 
		for (int i = 1; i <= Nx - 1; i++)
			for (int j = 1; j <= Ny - 1; j++)
			{

				U[i][j] = (psy[i][j + 1] - psy[i][j - 1]) / (2.0*dy);

				V[i][j] = -(psy[i + 1][j] - psy[i - 1][j]) / (2.0*dx);
			}
	
	
		for (int j = 0; j <= Ny; j++)
		{
			alpha[0] = 0.0;
			beta[0] = 1.0;
			for (int i = 1; i <= Nx - 1; i++)
			{
				
				A = (1.0 / (sqrt(pr*ra)*dx*dx))*(1.0 / (1.0 + (abs(U[i][j])*sqrt(pr*ra)*dx / 2.0))) - ((U[i][j]) / (2.0*dx)) + (abs(U[i][j]) / (2.0*dx));
				B = (2.0 / (sqrt(pr*ra)*dx*dx))*(1.0 / (1.0 + (abs(U[i][j])*sqrt(pr*ra)*dx / 2.0))) + (abs(U[i][j]) / (dx)) + (1.0 / dt);
				C = ((1.0 / (sqrt(pr*ra)*dx*dx))*(1.0 / (1.0 + (abs(U[i][j])*sqrt(pr*ra)*dx / 2.0)))) + (U[i][j] / (2.0*dx)) + (abs(U[i][j]) / (2.0*dx));
				F = -teta[i][j] / dt;
				alpha[i] = A/(B- alpha[i-1]*C);
				beta[i] = ((beta[i - 1] * C) - F) / (B - (alpha[i - 1] * C));
			}
			teta[Nx][j] = 0.0;
			for (int i = Nx - 1; i >= 0; i--)
				teta[i][j] = alpha[i] * teta[i + 1][j] + beta[i];
		}
		

		for (int i = 1; i <= Nx - 1; i++)
		{
			
			A = ((1.0 / (sqrt(pr*ra)*dy*dy))*(1.0 / (1.0 + (abs(V[i][1])*sqrt(pr*ra)*dy / 2.0)))) - (V[i][1] / (2.0*dy)) + (abs(V[i][1]) / (2.0*dy));
			B = ((2.0 / (sqrt(pr*ra)*dy*dy))*(1.0 / (1.0 + (abs(V[i][1])*sqrt(pr*ra)*dy / 2.0)))) + (abs(V[i][1]) / (dy)) + (1.0 / dt);
			C = ((1.0 / (sqrt(pr*ra)*dy*dy))*(1.0 / (1.0 + (abs(V[i][1])*sqrt(pr*ra)*dy / 2.0)))) + (V[i][1] / (2.0*dy)) + (abs(V[i][1]) / (2.0*dy));
			F = -teta[i][1] / dt;
			alpha[0] = (B-4.0*A)/(C-3.0*A);
			beta[0] = F/ (C - 3.0*A);
			
			for (int j = 1; j <= Ny - 1; j++)
			{
				
				A = ((1.0 / (sqrt(pr*ra)*dy*dy))*(1.0 / (1.0 + (abs(V[i][j])*sqrt(pr*ra)*dy / 2.0)))) - (V[i][j] / (2.0*dy)) + (abs(V[i][j]) / (2.0*dy));
				B = ((2.0 / (sqrt(pr*ra)*dy*dy))*(1.0 / (1.0 + (abs(V[i][j])*sqrt(pr*ra)*dy / 2.0)))) + (abs(V[i][j]) / (dy)) + (1.0 / dt);
				C = ((1.0 / (sqrt(pr*ra)*dy*dy))*(1.0 / (1.0 + (abs(V[i][j])*sqrt(pr*ra)*dy / 2.0)))) + (V[i][j] / (2.0*dy)) + (abs(V[i][j]) / (2.0*dy));
				F = -teta[i][j] / dt;
				alpha[j] = A / (B - (alpha[j - 1] * C));
				beta[j] = ((beta[j - 1] * C) - F) / (B - (alpha[j - 1] * C));
			}
			
			teta[i][Ny] = (4.0*beta[Ny - 1] - alpha[Ny - 2] * beta[Ny - 1] - beta[Ny - 1]) / (alpha[Ny - 2] * alpha[Ny - 1] - 4.0*alpha[Ny - 1] + 3.0);
			
			for (int j = Ny - 1; j >= 0; j--)
				teta[i][j] = alpha[j] * teta[i][j + 1] + beta[j];
		}
		for (int j = 1; j <= Ny-1; j++)
		{
			alpha[0] = 0.0;
			beta[0] = (psy[2][j] - 8.0*psy[1][j]) / (2.0*dx*dx);
			for (int i = 1; i <= Nx - 1; i++)
			{

				A = (sqrt(pr) / (sqrt(ra)*dx*dx))*(1.0 / (1.0 + (abs(U[i][j])*sqrt(ra)*dx / (2.0*sqrt(pr))))) - ((U[i][j]) / (2.0*dx)) + (abs(U[i][j]) / (2.0*dx));
				B = ((2.0*sqrt(pr)) / (sqrt(ra)*dx*dx))*(1.0 / (1.0 + (abs(U[i][j])*sqrt(ra)*dx / (2.0*sqrt(pr))))) + (abs(U[i][j]) / (dx)) + (1.0 / dt);
				C = ((sqrt(pr) / (sqrt(ra)*dx*dx))*(1.0 / (1.0 + (abs(U[i][j])*sqrt(ra)*dx / (2.0*sqrt(pr)))))) + (U[i][j] / (2.0*dx)) + (abs(U[i][j]) / (2.0*dx));
				F = (-omega[i][j] / dt)-((teta[i+1][j]-teta[i-1][j])/(2.0*dx));
				alpha[i] = A / (B - alpha[i - 1] * C);
				beta[i] = ((beta[i - 1] * C) - F) / (B - (alpha[i - 1] * C));
			}
			omega[Nx][j] = (psy[Nx - 2][j] - 8.0*psy[Nx - 1][j]) / (2.0*dx*dx);
			for (int i = Nx - 1; i >= 0; i--)
				omega[i][j] = alpha[i] * omega[i + 1][j] + beta[i];
		}


		for (int i = 1; i <= Nx - 1; i++)
		{
			alpha[0] = 0.0;
			beta[0] = (psy[i][2]-8.0*psy[i][1])/(2.0*dy*dy);

			for (int j = 1; j <= Ny - 1; j++)
			{

				A = ((sqrt(pr) / (sqrt(ra)*dy*dy))*(1.0 / (1.0 + (abs(V[i][j])*sqrt(ra)*dy / (2.0*sqrt(pr)))))) - (V[i][j] / (2.0*dy)) + (abs(V[i][j]) / (2.0*dy));
				B = (((2.0*sqrt(pr)) / (sqrt(ra)*dy*dy))*(1.0 / (1.0 + (abs(V[i][j])*sqrt(ra)*dy / (2.0*sqrt(pr)))))) + (abs(V[i][j]) / (dy)) + (1.0 / dt);
				C = ((sqrt(pr) / (sqrt(ra)*dy*dy))*(1.0 / (1.0 + (abs(V[i][j])*sqrt(ra)*dy / (2.0*sqrt(pr)))))) + (V[i][j] / (2.0*dy)) + (abs(V[i][j]) / (2.0*dy));
				F = -omega[i][j] / dt;
				alpha[j] = A / (B - (alpha[j - 1] * C));
				beta[j] = ((beta[j - 1] * C) - F) / (B - (alpha[j - 1] * C));
			}

			
			omega[i][Ny] = (psy[i][Ny-2] - 8.0*psy[i][Ny-1]) / (2.0*dy*dy);
			
			for (int j = Ny - 1; j >= 0; j--)
				omega[i][j] = alpha[j] * omega[i][j + 1] + beta[j];
		}
		for (int j = 0;j <= Ny;j++) 
		{
			Nu[j] = (3.0*teta[0][j] - 4.0*teta[1][j] + teta[2][j])/(2.0*dx);
		}
		double sum=0.0;
		
		for (int j = 1;j <= Ny-1;j++)
		{
			sum = sum + 2 * Nu[j];
		}
		Nus = (sum + Nu[0] + Nu[Ny])*(dy / 2);
		tt = tt+dx;
		
		
		fout4 << tt<<" "<<Nus<< endl;
		
	
	}
	
	x = 0.0; y = 0.0;
	ofstream fout("Resultomegapr7.txt");
	for (int i = 0; i <= Nx; i++)
	{
		x = i*dx;
		for (int j = 0; j <= Ny; j++)
		{
			y = j*dy;
			fout << x << " " << y << " " << omega[i][j] << endl;
		}
	}
	fout.close();
	x = 0.0; y = 0.0;
	ofstream fout1("Resultpsypr7.txt");
	for (int i = 0; i<= Nx; i++)
	{
		x = i*dx;
		for (int j = 0; j <= Ny; j++)
		{
			y = j*dy;
			fout1 << x << " " << y << " " << psy[i][j] << endl;
		}
	}
	fout1.close();
	x = 0.0; y = 0.0;
	ofstream fout2("Resulttetapr7.txt");
	for (int i = 0; i <= Nx; i++)
	{
		x = i*dx;
		for (int j = 0; j <= Ny; j++)
		{
			y = j*dy;
			fout2 << x << " " << y << " " << teta[i][j] << endl;
		}
	}
	fout2.close();
	x = 0.0; y = 0.0;
	ofstream fout3("ResultVUpr7.txt");
	for (int i = 0; i <= Nx; i++)
	{
		x = i*dx;
		for (int j = 0; j <= Ny; j++)
		{
			y = j*dy;
			fout3 << x << " " << y << " " << V[i][j] << " " << U[i][j] << endl;
		}
	}
	fout3.close();
	fout4.close();
	
	return 0;
}

