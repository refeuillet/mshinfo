#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libmeshb7.h"


/* simulate exception */
#include <setjmp.h>
jmp_buf ex_buf__;

extern const char *GmfKwdFmt[ GmfMaxKwd + 1 ][3];

int main(int argc, char *argv[])
{
  int        NmbVer, dim, deg, FilVer, NmbCor, NmbRidg;
  int        NmbTet, NmbHex, NmbPri, NmbPyr, NmbTri, NmbQua, NmbEdg;
  int        NmbTetP2, NmbHexQ2, NmbPriP2, NmbPyrP2, NmbTriP2, NmbQuaQ2, NmbEdgP2, NmbNod;
  int        NmbTetP3, NmbHexQ3, NmbPriP3, NmbPyrP3, NmbTriP3, NmbQuaQ3, NmbEdgP3;
  int        NmbTetP4, NmbHexQ4, NmbPriP4, NmbPyrP4, NmbTriP4, NmbQuaQ4, NmbEdgP4;
  int64_t    InpMsh, InpSol;
  int        i, j, ite = 0;
  float      flt;
  double     dbl, time = 0.0;
  int        NbrLin, SolSiz, NbrTyp, TypTab[ GmfMaxTyp ];
  

  deg    = 1;
  NmbNod = 0;


  if(argc <= 1){
    printf(" USAGE : mshinfo name [-check]\n");
    exit(1);
  }

  char name[1024];
  char sol[1024];

  strcpy(name,argv[1]);
  strcpy(sol,argv[1]);


  char *sub = NULL;
  sub = strstr(name,".mesh"); // check if it has the extension .mesh[b]


  char *subsol = NULL;
  subsol = strstr(name,".sol"); // check if it has the extension .sol[b]

// Mesh part

//-- test the reading of the file as a mesh
  if(subsol == NULL) { //--- no extension sol[b], check if it is a mesh name without extnsion
    if(sub != NULL) sol[sub-name]='\0'; // change file name to be opened as sol even if mesh extension is given

    if(sub != NULL)
      InpMsh = GmfOpenMesh(name, GmfRead, &FilVer, &dim);
    else {
      strcat(name,".meshb");
      InpMsh = GmfOpenMesh(name, GmfRead, &FilVer, &dim);
      if(InpMsh == 0){
        int ln = strlen(name);
        name[ln -1] = '\0';
        InpMsh = GmfOpenMesh(name, GmfRead, &FilVer, &dim);
      }
    }
  }

  if ( InpMsh != 0 && subsol == NULL ) {
    printf("Mesh informations :\n");
   /* Get number of entities*/
       
    NmbVer   = GmfStatKwd(InpMsh, GmfVertices);
    NmbCor   = GmfStatKwd(InpMsh, GmfCorners);
    NmbRidg  = GmfStatKwd(InpMsh, GmfRidges);
    NmbTet   = GmfStatKwd(InpMsh, GmfTetrahedra);
    NmbHex   = GmfStatKwd(InpMsh, GmfHexahedra);
    NmbPyr   = GmfStatKwd(InpMsh, GmfPyramids);
    NmbPri   = GmfStatKwd(InpMsh, GmfPrisms);
    NmbTri   = GmfStatKwd(InpMsh, GmfTriangles);
    NmbQua   = GmfStatKwd(InpMsh, GmfQuadrilaterals);
    NmbEdg   = GmfStatKwd(InpMsh, GmfEdges);
    NmbTetP2 = GmfStatKwd(InpMsh, GmfTetrahedraP2);
    NmbHexQ2 = GmfStatKwd(InpMsh, GmfHexahedraQ2);
    NmbPyrP2 = GmfStatKwd(InpMsh, GmfPyramidsP2);
    NmbPriP2 = GmfStatKwd(InpMsh, GmfPrismsP2);
    NmbTriP2 = GmfStatKwd(InpMsh, GmfTrianglesP2);
    NmbQuaQ2 = GmfStatKwd(InpMsh, GmfQuadrilateralsQ2);
    NmbEdgP2 = GmfStatKwd(InpMsh, GmfEdgesP2);
    NmbTetP3 = GmfStatKwd(InpMsh, GmfTetrahedraP3);
    NmbHexQ3 = GmfStatKwd(InpMsh, GmfHexahedraQ3);
    NmbPyrP3 = GmfStatKwd(InpMsh, GmfPyramidsP3);
    NmbPriP3 = GmfStatKwd(InpMsh, GmfPrismsP3);
    NmbTriP3 = GmfStatKwd(InpMsh, GmfTrianglesP3);
    NmbQuaQ3 = GmfStatKwd(InpMsh, GmfQuadrilateralsQ3);
    NmbEdgP3 = GmfStatKwd(InpMsh, GmfEdgesP3);
    NmbTetP4 = GmfStatKwd(InpMsh, GmfTetrahedraP4);
    NmbHexQ4 = GmfStatKwd(InpMsh, GmfHexahedraQ4);
    NmbPyrP4 = GmfStatKwd(InpMsh, GmfPyramidsP4);
    NmbPriP4 = GmfStatKwd(InpMsh, GmfPrismsP4);
    NmbTriP4 = GmfStatKwd(InpMsh, GmfTrianglesP4);
    NmbQuaQ4 = GmfStatKwd(InpMsh, GmfQuadrilateralsQ4);
    NmbEdgP4 = GmfStatKwd(InpMsh, GmfEdgesP4);

    printf("version = %d; dim = %d; nbv = %d; (nbc = %d)\n", FilVer, dim, NmbVer, NmbCor);
    
    if ( NmbTet >0 || NmbTri > 0 || NmbEdg > 0 || NmbHex > 0|| NmbPri > 0 || NmbPyr > 0|| NmbQua > 0 ) {
      printf("P1/Q1 elements :\n");
      if ( NmbTet >0 || NmbTri > 0 || NmbEdg > 0 ) 
        printf("nbtet = %d; nbtri = %d; nbedg = %d; (nbrdg = %d)\n", NmbTet, NmbTri, NmbEdg, NmbRidg);
      if ( NmbHex > 0|| NmbPri > 0 || NmbPyr > 0|| NmbQua > 0 )
        printf("nbhex = %d; nbpri = %d; nbpyr = %d; nbqua = %d\n", NmbHex, NmbPri, NmbPyr, NmbQua);
    }
    
    if ( NmbTetP2 >0 || NmbTriP2 > 0 || NmbEdgP2 > 0 || NmbHexQ2 > 0|| NmbPriP2 > 0 || NmbPyrP2 > 0|| NmbQuaQ2 > 0 ) {
      printf("P2/Q2 elements :\n"); 
      if ( NmbTetP2 >0 || NmbTriP2 > 0 || NmbEdgP2 > 0 )
        printf("nbtetP2 = %d; nbtriP2 = %d; nbedgP2 = %d; (nbrdg = %d)\n", NmbTetP2, NmbTriP2, NmbEdgP2, NmbRidg);
      if ( NmbHexQ2 > 0|| NmbPriP2 > 0 || NmbPyrP2 > 0|| NmbQuaQ2 > 0 )
        printf("nbhexQ2 = %d; nbpriP2 = %d; nbpyrP2 = %d; nbquaQ2 = %d\n", NmbHexQ2, NmbPriP2, NmbPyrP2, NmbQuaQ2);
    }
    
    if ( NmbTetP3 >0 || NmbTriP3 > 0 || NmbEdgP3 > 0 || NmbHexQ3 > 0|| NmbPriP3 > 0 || NmbPyrP3 > 0|| NmbQuaQ3 > 0 ) {
      printf("P3/Q3 elements :\n"); 
      if ( NmbTetP3 >0 || NmbTriP3 > 0 || NmbEdgP3 > 0 )
        printf("nbtetP3 = %d; nbtriP3 = %d; nbedgP3 = %d; (nbrdg = %d)\n", NmbTetP3, NmbTriP3, NmbEdgP3, NmbRidg);
      if ( NmbHexQ3 > 0|| NmbPriP3 > 0 || NmbPyrP3 > 0|| NmbQuaQ3 > 0 )
        printf("nbhexQ3 = %d; nbpriP3 = %d; nbpyrP3 = %d; nbquaQ3 = %d\n", NmbHexQ3, NmbPriP3, NmbPyrP3, NmbQuaQ3);
    }

    if ( NmbTetP4 >0 || NmbTriP4 > 0 || NmbEdgP4 > 0 || NmbHexQ4 > 0|| NmbPriP4 > 0 || NmbPyrP4 > 0|| NmbQuaQ4 > 0 ) {
      printf("P4/Q4 elements :\n"); 
      if ( NmbTetP4 >0 || NmbTriP4 > 0 || NmbEdgP4 > 0 )
        printf("nbtetP4 = %d; nbtriP4 = %d; nbedgP4 = %d; (nbrdg = %d)\n", NmbTetP4, NmbTriP4, NmbEdgP4, NmbRidg);
      if ( NmbHexQ4 > 0|| NmbPriP4 > 0 || NmbPyrP4 > 0|| NmbQuaQ4 > 0 )
        printf("nbhexQ4 = %d; nbpriP4 = %d; nbpyrP4 = %d; nbquaQ4 = %d\n", NmbHexQ4, NmbPriP4, NmbPyrP4, NmbQuaQ4);
    }

    GmfCloseMesh(InpMsh);
     
  }
  

  // solution part

//-- test the reading of the file as a solution
  if (subsol != NULL) { //--- a file containing char ".sol" is given in input
    InpSol = GmfOpenMesh(sol, GmfRead, &FilVer, &dim);
    if ( InpSol == 0 ) {
      strcat(sol,"b");
      InpSol = GmfOpenMesh(sol, GmfRead, &FilVer, &dim);
    }
  }
  else { //-- check if a solution with the same name as the mesh one exists
    strcat(sol,".sol");
    InpSol = GmfOpenMesh(sol, GmfRead, &FilVer, &dim);
    if ( InpSol == 0 ) {
      strcat(sol,"b");
      InpSol = GmfOpenMesh(sol, GmfRead, &FilVer, &dim);
    }
  }

  if (InpSol!= 0 ) {
    printf("Solution informations :\n");

    GmfGotoKwd(InpSol, GmfTime);
    if ( FilVer == GmfFloat ) {	// read 32 bits float
     GmfGetLin(InpSol, GmfTime, &flt);
     time = (double)flt;
    }  
    else if ( FilVer == GmfDouble ) {	// read 64 bits float
      GmfGetLin(InpSol, GmfTime, &dbl);
      time = dbl;
    }
    if ( GmfStatKwd(InpSol, GmfIterations) ) {
      GmfGotoKwd(InpSol, GmfIterations);
      GmfGetLin( InpSol, GmfIterations, &ite);  
    }  
    printf("dim = %d; ite = %d; time = %lg\n",dim, ite, time);
    for(i=1; i<=GmfMaxKwd; i++) {
      if( ( (!strcmp(GmfKwdFmt[i][2], "sr")  || !strcmp(GmfKwdFmt[i][2], "hr")) ) && ( (NbrLin = GmfStatKwd(InpSol, i, &NbrTyp, &SolSiz, TypTab, &deg, &NmbNod)) ) ) {
        printf("%s = %d\n", GmfKwdFmt[i][0], NbrLin);
        if ( deg != 1 && NmbNod != 0 )
          printf("deg = %d nbnod = %d\n",deg, NmbNod);
        printf("type = [");
        for(j=0; j<NbrTyp; j++) {
          if (      TypTab[j] == 1 ) printf(" scalar ");
          else if ( TypTab[j] == 2 ) printf(" vector ");
          else if ( TypTab[j] == 3 ) printf(" metric ");
          else if ( TypTab[j] == 4 ) printf(" matrix ");
        }
        printf("]\n");
        deg = 1;
        NmbNod = 0;
      }
    }
    GmfCloseMesh(InpSol);
  }

  if ( InpSol == 0 && InpMsh == 0 ) {
    printf("cannot open mesh/solution file %s\n", argv[1]);
    exit(1);
  }

  return(0);

}
