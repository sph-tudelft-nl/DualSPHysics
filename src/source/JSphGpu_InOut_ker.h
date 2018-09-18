//HEAD_DSPH
/*
 <DUALSPHYSICS>  Copyright (c) 2017 by Dr Jose M. Dominguez et al. (see http://dual.sphysics.org/index.php/developers/). 

 EPHYSLAB Environmental Physics Laboratory, Universidade de Vigo, Ourense, Spain.
 School of Mechanical, Aerospace and Civil Engineering, University of Manchester, Manchester, U.K.

 This file is part of DualSPHysics. 

 DualSPHysics is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License 
 as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.
 
 DualSPHysics is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details. 

 You should have received a copy of the GNU Lesser General Public License along with DualSPHysics. If not, see <http://www.gnu.org/licenses/>. 
*/

/// \file JSphGpu_InOut_ker.h \brief Declares functions and CUDA kernels for InOut feature.

#ifndef _JSphGpu_InOut_ker_
#define _JSphGpu_InOut_ker_

#include "Types.h"
#include <cuda_runtime_api.h>

/// Implements a set of functions and CUDA kernels for InOut feature.
namespace cusphinout{

inline float3 Float3(const tfloat3& v){ float3 p={v.x,v.y,v.z}; return(p); }
inline float3 Float3(float x,float y,float z){ float3 p={x,y,z}; return(p); }
inline float4 Float4(const tfloat4& v){ float4 p={v.x,v.y,v.z,v.w}; return(p); }
inline tfloat3 ToTFloat3(const float3& v){ return(TFloat3(v.x,v.y,v.z)); }
inline double3 Double3(const tdouble3& v){ double3 p={v.x,v.y,v.z}; return(p); }
inline double4 Double4(const tdouble4& v){ double4 p={v.x,v.y,v.z,v.w}; return(p); }

//-Kernels for inlet/outlet (JSphInOut).
void UpdatePosFluid(byte periactive,unsigned n,unsigned pini
  ,double2 *posxy,double *posz,unsigned *dcell,typecode *code);
unsigned InOutCreateList(bool stable,unsigned n,unsigned pini
  ,byte convertfluidmask,byte nzone,const byte *cfgzone,const float4 *planes
  ,tfloat3 freemin,tfloat3 freemax
  ,const float2 *boxlimit,const double2 *posxy,const double *posz
  ,typecode *code,unsigned *listp);
void InOutUpdateData(unsigned n,const unsigned *listp
  ,byte izone,byte rmode,byte vmode,byte vprof
  ,float timestep,float zsurf,tfloat4 veldata,tfloat4 veldata2,tfloat3 dirdata
  ,float coefhydro,float rhopzero,float gamma
  ,const typecode *code,const double *posz,float4 *velrhop);

void InoutClearInteractionVars(unsigned n,const int *inoutpart
    ,float3 *ace,float *ar,float *viscdt,float3 *shiftpos);

void InOutUpdateVelrhopM1(unsigned n,const int *inoutpart
    ,const float4 *velrhop,float4 *velrhopm1);

void InOutComputeStep(byte periactive,unsigned n,int *inoutpart
  ,double dt,const float4 *planes,const float *width,const float4 *velrhop
  ,double2 *posxy,double *posz,unsigned *dcell,typecode *code);
unsigned InOutListCreate(bool stable,unsigned n,unsigned nmax,int *inoutpart);
void InOutCreateNewInlet(byte periactive,unsigned newn,const unsigned *newinoutpart
  ,unsigned np,unsigned idnext,typecode codenewpart,const float3 *dirdata,const float *width
  ,double2 *posxy,double *posz,unsigned *dcell,typecode *code,unsigned *idp,float4 *velrhop);

//-Kernels for inlet/outlet filling (JSphInOut).
void InOutFillMove(byte periactive,unsigned n,const unsigned *inoutpart
  ,double dt,const float4 *velrhop
  ,double2 *posxy,double *posz,unsigned *dcell,typecode *code);
void InOutFillProjection(unsigned n,const unsigned *inoutpart
  ,typecode codenewpart,const float4 *planes,const float *width
  ,const double2 *posxy,const double *posz
  ,typecode *code,float *prodist,double2 *proposxy,double *proposz);
void InOutRemoveZsurf(unsigned n,const unsigned *inoutpart
  ,typecode codezone,float zsurf,const double *posz
  ,typecode *code,float *prodist,double2 *proposxy,double *proposz);
unsigned InOutFillListCreate(bool stable,unsigned npt
  ,const double2 *ptposxy,const double *ptposz
  ,const byte *ptzone,const float *zsurf,const float *width
  ,unsigned npropt,const float *prodist,const double2 *proposxy,const double *proposz
  ,float dpmin,float dpmin2,float dp,float *ptdist,unsigned nmax,unsigned *inoutpart);
void InOutFillCreate(byte periactive,unsigned newn,const unsigned *newinoutpart
  ,const double2 *ptposxy,const double *ptposz,const byte *ptzone,const float *ptauxdist
  ,unsigned np,unsigned idnext,typecode codenewpart,const float3 *dirdata
  ,double2 *posxy,double *posz,unsigned *dcell,typecode *code,unsigned *idp,float4 *velrhop);

//-Kernels for Zsurf calculation (JSphInOut).
float InOutComputeZsurf(unsigned nptz,const float3 *ptzpos,float maxdist,float zbottom
  ,TpCellMode cellmode,tuint3 ncells,const int2 *begincell,tuint3 cellmin
  ,const double2 *posxy,const double *posz,const typecode *code
  ,float *resg,float *resh);

//-Kernels to extrapolate rhop and velocity (JSphInOut).
void Interaction_InOutExtrap(bool usedouble,bool simulate2d,TpKernel tkernel,TpCellMode cellmode
  ,unsigned inoutcount,const int *inoutpart,const byte *cfgzone,byte computerhopmask,byte computevelmask
  ,const float4 *planes,const float* width,const float3 *dirdata,float determlimit
  ,tuint3 ncells,const int2 *begincell,tuint3 cellmin
  ,const double2 *posxy,const double *posz,const typecode *code,const unsigned *idp
  ,float4 *velrhop);

//-Kernels to extrapolate rhop on boundary particles (JSphBoundCorr).
void Interaction_BoundCorr_Double(bool simulate2d,TpKernel tkernel,TpCellMode cellmode
  ,unsigned npbok,typecode boundcode,tfloat4 plane,tfloat3 direction,float determlimit
  ,tuint3 ncells,const int2 *begincell,tuint3 cellmin
  ,const double2 *posxy,const double *posz,const typecode *code,const unsigned *idp
  ,float4 *velrhop);

//-Kernels to interpolate velocity (JSphInOutGridDataTime).
void InOutInterpolateTime(unsigned npt,double time,double t0,double t1
  ,const float *velx0,const float *velx1,float *velx
  ,const float *velz0,const float *velz1,float *velz);
void InOutInterpolateZVel(unsigned izone,double posminz,double dpz,int nz1
  ,const float *velx,const float *velz
  ,unsigned np,const int *plist,const double *posz,const typecode *code,float4 *velrhop);
void InOutInterpolateResetZVel(unsigned izone,unsigned np,const int *plist
  ,const typecode *code,float4 *velrhop);

}


#endif


