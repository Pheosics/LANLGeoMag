#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <Lgm_CTrans.h>
#include <Lgm_QinDenton.h>
#include <Lgm_ElapsedTime.h>
#include <Lgm_FluxToPsd.h>
#include <omp.h>

int ClassifyFL_Enhanced( int Flag, Lgm_Vector *w ) {

    int NewFlag = 0;

    if ( Flag < 0 ) return( Flag );

    switch (Flag) {

            case LGM_OPEN_IMF:
                                break;
            case LGM_CLOSED:
                                if ( w->x > 0.0 ) {
                                    NewFlag = 1;
                                } else {
                                    NewFlag = ( fabs(w->y) > 10.0 ) ? 2 : 3;
                                    //NewFlag = 2+fabs( w->x )*10.0;
                                }
                                break;
            case LGM_OPEN_N_LOBE:
                                NewFlag = ( (fabs(w->z) < 20.0) && (fabs(w->y) > 10.0 ) ) ? 5 : 4;
                                NewFlag = ( fabs(w->z) < 20.0 ) ? 5 : 4;
printf("w = %g %g %g NewFlag = %d\n", w->x, w->y, w->z, NewFlag);
                                break;
            case LGM_OPEN_S_LOBE:
                                NewFlag = ( fabs(w->z) < 10.0 ) ? 6 : 4;
                                break;


    }

    return( NewFlag );

}

int main(){

    int                 NX, NY, i, j;
    double              LX_MIN, LX_MAX;
    double              LY_MIN, LY_MAX;
    double              x, y, GeodLat, GeodLong, GeodHeight ;
    double              UTC, JD;
    long int            Date;
    int                 Flag;
    double              **Image;
    Lgm_QinDentonOne    p;
    Lgm_Vector          u, v, v1, v2, v3, w;
    Lgm_MagModelInfo    *mInfo2;
    Lgm_MagModelInfo    *mInfo = Lgm_InitMagInfo();
    

    NX     = 1200;
    LX_MIN = -30.0;
    LX_MAX =  30.0;

    NY     = 1200;
    LY_MIN = -30.0;
    LY_MAX =  30.0;

    GeodHeight = 100.0;  //km

    Date = 20121010;
    UTC  = 16.0 + 27.0/60.0 + 29.99/3600.0;
    JD = Lgm_Date_to_JD( Date, UTC, mInfo->c );
    Lgm_Set_Coord_Transforms( Date, UTC, mInfo->c );
    Lgm_get_QinDenton_at_JD( JD, &p, 1 );
    Lgm_set_QinDenton( &p, mInfo );

    Lgm_MagModelInfo_Set_MagModel( LGM_IGRF, LGM_EXTMODEL_TS04, mInfo );


    LGM_ARRAY_2D( Image, NX, NY, double );

    { // BEGIN PARALLEL EXECUTION
        #pragma omp parallel private(x,y,j,GeodLat,GeodLong,u,v,v1,v2,v3,Flag,mInfo2)
        #pragma omp for schedule(dynamic, 1)
        for ( i=0; i<NX; i++ ) {
            x = (LX_MAX-LX_MIN) * i / ((double)(NX-1)) + LX_MIN;

            mInfo2 = Lgm_CopyMagInfo( mInfo );

            printf("i=%d\n", i);
            
            for ( j=0; j<NY; j++ ) {
                y = (LY_MAX-LY_MIN) * j / ((double)(NY-1)) + LY_MIN;

                GeodLat  = 90.0 - sqrt( x*x + y*y );
                GeodLong = atan2( y, x )*DegPerRad;


                Lgm_GEOD_to_WGS84( GeodLat, GeodLong, GeodHeight, &v );
                Lgm_Convert_Coords( &v, &u, WGS84_TO_GSM, mInfo->c );

                Flag = Lgm_Trace( &u, &v1, &v2, &v3, GeodHeight, 1e-7, 1e-7, mInfo2 );
                Lgm_Convert_Coords( &v3, &w, GSM_TO_SM, mInfo->c );
                Image[i][j] = (double)ClassifyFL_Enhanced(Flag, &w)+100;

                //printf("x, y, = %g %g GeodLat, GeodLong, GeodHeight = %g %g %g     u = %g %g %g   Flag = %d\n", x, y, GeodLat, GeodLong, GeodHeight, u.x, u.y, u.z, Flag);

            }

            Lgm_FreeMagInfo( mInfo2 );

        }
    } // END PARALLEL EXECUTION



    DumpGif( "Image_TS04", NX, NY, Image );
    LGM_ARRAY_2D_FREE( Image );

    Lgm_FreeMagInfo( mInfo );
    return(0);


}