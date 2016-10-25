#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#define MAX_ITERATION 10 



char DAT_FILE[100]; /* locations of sensor nodes in the target field, stored in a data file */
int n; /* total number of sensor nodes to be deployed */
double RANGE; /* communication range of sensor node */
int sensor_field[500][500][2];


typedef struct sensor{
	int id;
	int x; /* x-coordinate of sensor node location in target field */
	int y; /* y-coordinate of sensor node location in target field */
	int totPhyNeighbourSize; /* number of physical neighbors of a sensor node */
	int PhyNeighbour[150]; /* list of physical neighbors of a sensor node */
	int key[150];
} sensor;

sensor *s;/*Set of sensors*/

long int ModPower(long int x, long int e, long int n)
{

	// To calculate y:=x^e(mod n).
	long int i = pow(x,e);
	long int y;
	y = i % n;
	return y;

}

int isPrimitive (long int q, int a) {
	long int i,z;
	long double k,s;

	s=1;
	i=0;

	long double *factors=malloc(sizeof(double)*q-2);

	while (s>0 && i<q-2) {
		k=pow(a,i);
		s=k-(q*floor(k/q));
		for (z=0;z<(sizeof(*factors)/sizeof(factors[0]));z++) {
			if (factors[z]==s) {
				return 0;
			}
		}
		factors[i]=s;
		i++;
	}

	if (i==q-2) {
		return 1;
	} else {
		return 0;
	}







}




int MillerRobinTest(long int n, int iteration)
{
	long int m, t;
	int i,j;
	long int a, u;
	int flag;
	if(n % 2 == 0)
		return 0; // n is composite.
	m = (n-1) / 2;
	t = 1;
	while( m % 2 == 0) // repeat until m is even
	{
		m = m / 2;
		t = t + 1;
	}

	for (j=0; j < iteration; j++) { // Repeat the test for MAX_ITERATION times
		flag = 0;
		srand((unsigned int) time(NULL));
		a = random() % n + 1; // select a in {1,2,......,n}
	u = ModPower(a,m,n);
	if (u == 1 || u == n - 1)
		flag = 1;
	for(i=0;i<t;i++)
	{
		if(u == n - 1)
			flag = 1;
		u = (u * u) % n;
	}
	if ( flag == 0 )
		return 0; // n is composite
	}
	return 1; // n is prime.
} // end of MillerRobinTest().

/*

   void get_sensors_count()
   {
   FILE *fp;
   fp = fopen(DAT_FILE, "r");
   fscanf(fp, "%d", &n);
   fclose(fp);
   }
   */
void read_sensor_data()
{
	FILE *fp;
	int i, a, b;
	memset(sensor_field, 0, sizeof(int)*250*250*2);
	fp = fopen(DAT_FILE, "r");
	//fscanf(fp, "%d", &n);
	for(i = 0; i < n; i++)
	{
		fscanf(fp, "%d %d", &a, &b);
		printf("here i: %d, a : %d, b: %d \n",i , a, b);
		sensor_field[a][b][0] = 1;//presence of the sensor
		sensor_field[a][b][1] = i;//id of the sensor
		s[i].id = i;
		s[i].x = a;
		s[i].y = b;
	}
	fclose(fp);
}


void find_phy_neighbours()
{
	/*assuming that the range is square, so searching 
	 * for physical neighbours in 25m^2 area*/
	int i, a, b, node_id, count, avg = 0;
	for(i = 0; i < n; i++)
	{
		count = 0;
		int px, qx, py, qy;
		px = (s[i].x - 25)>0?(s[i].x - 25):0;
		qx = (s[i].x + 25)<500?(s[i].x + 25):499;
		py = (s[i].y - 25)>0?(s[i].y - 25):0;
		qy = (s[i].y + 25)<500?(s[i].y + 25):499;
		for(a = px; a <= qx; a++)
		{
			for(b = py; b <= qy; b++)
			{
				if((a == s[i].x) && (b == s[i].y))
					continue;
				if(sensor_field[a][b][0] == 1)
				{
					node_id = sensor_field[a][b][1];
					s[i].PhyNeighbour[count] = node_id;
					count++;
				}
			}
		}
		s[i].totPhyNeighbourSize = count;
	}

}


void create_plot()
{
	FILE *fp;
	int i;
	char command[100];
	fp = fopen("sensors.data", "w+");
	for(i = 0; i < n; i++)
		fprintf(fp, "%d %d\n", s[i].x, s[i].y);
	fclose(fp);
	fp = fopen("gnuplot_script", "w+");
	fprintf(fp, "set term postscript;\n"); 
	fprintf(fp, "set output \"deployment.ps\";\n");
	fprintf(fp, "plot \"sensors.data\" with points;\n");
	fclose(fp);
	strcpy(command, "gnuplot gnuplot_script; ps2pdf deployment.ps; rm gnuplot_script sensors.data; rm deployment.ps");
	system(command);
}

int main(int argc, char *argv[])
{
	srand(time(NULL));

	/*********Initialise data**************/
	strcpy(DAT_FILE, "sensor.dat");
	RANGE = 25;
	//get_sensors_count();
	printf("value of LAMBDA is one tenth the number of sensor nodes\n");
	printf("Enter number of sensor nodes (n) in 500*500 area (Max Limit = 10,000): ");
	scanf("%d", &n);
	printf("n is %d\n", n);
	if(n > 10000){
		printf("Max Limit for number of sensors exceede :( \n");
		return 0 ;
	}

	int LAMBDA = n/10;


	printf("Initiating Sensor Objects\n");
	s = (sensor *)malloc(sizeof(sensor)*n);
	int i, j ,k;

	printf("Performing MillerRobinTest\n");
	long int q;
	// initializing min value of prime
	q = pow(2, 10);
	while(1)
	{
		q++;
		/* test for even number */
		if ( q & 0x01 == 0 ) continue;
		if(MillerRobinTest(q, MAX_ITERATION))
			break;
	}

	printf("found prime to be : %ld \n", q);
	printf("finding primitive root for Vandermond Matrix\n");


	long int g  = 2;
	long int result = 0;
	long int t;

	   long int hash[q];
	   long int flag,temp=1,temp2;
	   for(g=2; g<q; g++)
	   {
	   memset(hash, 0, sizeof(hash));
	   for(t=1; t<q; t++)
	   {
   	   //printf("Inside1 : \n");

	   long int temp =ModPower( g,t,q);  // To calculate y:=g^t(mod q).
	   //printf("Inside2 temp : %ld \n",g);
	   if(temp < 0){
	   	temp = temp + q;
	   }
	   //printf("Inside2 t : %ld , temp : %ld \n", t, temp);
	   hash[temp]=1;
	   }
		//printf("outside : \n");

	   flag=1;
	   for(t=1; t<q; t++)
	   {
	   if(hash[t]!=1)
	   flag=0;
	   }
	   if(flag==1)
	   break;
	   }


/*
	while (result==0 ){
		result=isPrimitive(q,g);
		if(result ==0){
			g++;
			continue;
		}
		else{
			break;
		}
	}

*/
	printf("found primitive to be : %d \n", g);

	double	matrixG[LAMBDA+1][n];

	for(i=0;i<LAMBDA+1;i++){
		int temp1 = pow(g,i);
		for(j=0;j<n;j++){
			matrixG[i][j] =  pow(temp1, (j+1));
		}
	}

	double	matrixD[LAMBDA+1][LAMBDA+1];

	for(i=0;i<LAMBDA+1;i++){
		for(j=i;j<LAMBDA+1;j++){
			matrixD[i][j] =  rand() % q;
			matrixD[j][i] =  matrixD[i][j];		
		}
	}

	double	matrixAt[LAMBDA+1][n];
	double	matrixA[n][LAMBDA+1];


	for(i=0; i<LAMBDA+1; ++i)
		for(j=0; j<n; ++j)
			for(k=0; k<LAMBDA+1; ++k)
			{
				matrixAt[i][j]+=matrixD[i][k]*matrixG[k][j];
			}



	for(i=0; i<LAMBDA+1; ++i)
		for(j=0; j<n; ++j)
		{
			matrixA[j][i] = matrixAt[i][j];
		}






	printf("Reading Sensor Data File...\n");
	read_sensor_data();

	int Arow[n][LAMBDA+1];
	int Gcolumn[n][LAMBDA+1];

	for(i =0;i<n;i++){
		for(j=0;j<LAMBDA+1;j++){
			Arow[i][j] = matrixA[i][j];
			Gcolumn[i][j] = matrixG[j][i];
		}
	}



	printf("Plotting Sensors...\n");
	create_plot();

	/*******Key Pre-distribution Phase*******/
	printf("Initiating Key Pre-Dristribution Phase\n");

	printf("Searching for Physical Neighbours\n");
	find_phy_neighbours();
	printf("Physical Neighbours Located\n");

	for(i=0;i<n;i++){
		for(j=0;j<s[i].totPhyNeighbourSize;j++){
			s[i].key[j] = 0;
			for(k=0;k<LAMBDA+1;k++){
				s[i].key[j] = s[i].key[j] + Arow[i][k] * Gcolumn[s[i].PhyNeighbour[j]][k];
			}
		}
	}

	printf("Generating Neighbours and Keys to keys.txt ..... \n");
	FILE *fp;
	fp = fopen("keys.txt", "w+");
	for(i = 0; i < n; i++){
		fprintf(fp, "Node %d : NL%d = { ", i, i);
		for(j=0;j<s[i].totPhyNeighbourSize;j++){
			fprintf(fp, " %d,", s[i].PhyNeighbour[j]);
		}
		fprintf(fp, "}\n");
		for(j=0;j<s[i].totPhyNeighbourSize;j++){
			fprintf(fp, "Key(%d,%d) = %d, ", i, s[i].PhyNeighbour[j], s[i].key[j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);


	return 0;
}
