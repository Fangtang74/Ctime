/*Author: Lu Wei" */
#include <iostream>
#include <ctime>
#include <sys/timeb.h>

/*

   For MR_PAIRING_CP curve
   cl /O2 /GX bls.cpp cp_pair.cpp zzn2.cpp big.cpp zzn.cpp ecn.cpp miracl.lib
   (Note this really doesn't make much sense as the signature will not be "short")

   For MR_PAIRING_MNT curve
   cl /O2 /GX bls.cpp mnt_pair.cpp zzn6a.cpp ecn3.cpp zzn3.cpp zzn2.cpp big.cpp zzn.cpp ecn.cpp miracl.lib

   For MR_PAIRING_BN curve
   cl /O2 /GX bls.cpp bn_pair.cpp zzn12a.cpp ecn2.cpp zzn4.cpp zzn2.cpp big.cpp zzn.cpp ecn.cpp miracl.lib

   For MR_PAIRING_KSS curve
   cl /O2 /GX bls.cpp kss_pair.cpp zzn18.cpp zzn6.cpp ecn3.cpp zzn3.cpp big.cpp zzn.cpp ecn.cpp miracl.lib

   For MR_PAIRING_BLS curve
   cl /O2 /GX bls.cpp bls_pair.cpp zzn24.cpp zzn8.cpp zzn4.cpp zzn2.cpp ecn4.cpp big.cpp zzn.cpp ecn.cpp miracl.lib

   Test program
*/

//********* choose just one of these pairs **********
//#define MR_PAIRING_CP      // AES-80 security
//#define AES_SECURITY 80

//#define MR_PAIRING_MNT	// AES-80 security
//#define AES_SECURITY 80

#define MR_PAIRING_BN    // AES-128 or AES-192 security
#define AES_SECURITY 128
//#define AES_SECURITY 192

//#define MR_PAIRING_KSS    // AES-192 security
//#define AES_SECURITY 192

//#define MR_PAIRING_BLS    // AES-256 security
//#define AES_SECURITY 256
//*********************************************

#include "pairing_3.h"
#include "miracl.h"


#include "bn_pair.cpp"
#include "zzn12a.cpp"
#include "ecn2.cpp"
#include "zzn4.cpp"
#include "zzn2.cpp"
#include "big.cpp"
#include "zzn.cpp"
#include "ecn.cpp"
#include "mrshs.c"



static Big Hash(const string& message) {
    char s[20];
    Big num;
    sha sh;
    shs_init(&sh);
    for(auto &ele:message) {
        shs_process(&sh,ele);
    }
    shs_hash(&sh,s);
    num=from_binary(20,s);
    return num;
}

void measure_hash() {
	long sum=0,t=0;
	timeb t1,t2;
    PFC pfc(AES_SECURITY);
	for(int i=0;i<5000;i++) {
		ftime(&t1);
		Big test=Hash(string("hahahahahahahahaha"));
		ftime(&t2);
		t=(t2.time-t1.time)*1000+(t2.millitm-t1.millitm);
		sum+=t;
	}
	cout<<"measure_hash: "<<sum<<endl;
}

void measure_bsm(){
	timeb t1,t2;
    long t;
	PFC pfc(AES_SECURITY);  // initialise pairing-friendly curve

	G2 Q,V;
	G1 S,R;
	GT result;
	Big s,X;
	time_t seed;

	time(&seed);
    irand((long)seed);

	long sum=0;
    for (int i = 0; i < 5000; ++i) {
        s=rand(pfc.order());
        pfc.random(Q);
        ftime(&t1);
        V=pfc.mult(Q,s);
        ftime(&t2);
        t=(t2.time-t1.time)*1000+(t2.millitm-t1.millitm);
        sum+=t;
    }
    cout<<"measure_bsm: "<<sum<<endl;
}

void measure_bp() {
	timeb t1,t2;
    long t;
	PFC pfc(AES_SECURITY);  // initialise pairing-friendly curve

	G2 Q,V;
	G1 S,R;
	GT result;
	Big s,X;
	time_t seed;

	time(&seed);
    irand((long)seed);
	
	long sum=0;
    for(int i=0;i<1000;i++) {
        pfc.random(S);
        pfc.random(V);
        ftime(&t1);
        result=pfc.pairing(V,S);
        ftime(&t2);
        t=(t2.time-t1.time)*1000+(t2.millitm-t1.millitm);
        sum+=t;
    }
    cout<<"measure_bp: "<<sum<<endl;
}

void measure_gtmul() {
	timeb t1,t2;
    long t;
	PFC pfc(AES_SECURITY);  // initialise pairing-friendly curve

	G2 Q,V,_Q,_V;
	G1 S,R,_S;
	GT result,_result,temp;
	Big s,X;
	time_t seed;

	time(&seed);
    irand((long)seed);
	
	long sum=0;
    for(int i=0;i<1000;i++) {
        pfc.random(S);
        pfc.random(_S);
        pfc.random(V);
        pfc.random(_V);
        result=pfc.pairing(V,S);
        _result=pfc.pairing(_V,_S);
        ftime(&t1);
        temp=result*_result;
        ftime(&t2);
        t=(t2.time-t1.time)*1000+(t2.millitm-t1.millitm);
        sum+=t;
    }
    cout<<"measure_gtmul: "<<sum<<endl;
}

void measure_gtexp() {
	timeb t1,t2;
    long t;
	PFC pfc(AES_SECURITY);  // initialise pairing-friendly curve

	G2 Q,V;
	G1 S,R;
	GT result,temp;
	Big s,X,pow;
	time_t seed;

	time(&seed);
    irand((long)seed);
	
	long sum=0;
    for(int i=0;i<1000;i++) {
        pfc.random(S);
        pfc.random(V);
        result=pfc.pairing(V,S);
        pow=rand(pfc.order());
        ftime(&t1);
        temp=pfc.power(result,pow);
        ftime(&t2);
        t=(t2.time-t1.time)*1000+(t2.millitm-t1.millitm);
        sum+=t;
    }
    cout<<"measure_gtexp: "<<sum<<endl;
}

void measure_bap() {
	timeb t1,t2;
    long t;
	PFC pfc(AES_SECURITY);  // initialise pairing-friendly curve

	G2 Q,V;
	G1 S,R;
	GT result;
	Big s,X;
	time_t seed;

	time(&seed);
    irand((long)seed);
	
	long sum=0;
    for(int i=0;i<5000;i++) {
        pfc.random(S);
        pfc.random(R);
        G1 SR;
        ftime(&t1);
        SR=S+R;
        ftime(&t2);
        t=(t2.time-t1.time)*1000+(t2.millitm-t1.millitm);
        sum+=t;
    }
    cout<<"measure_bap: "<<sum<<endl;
}

void measure_mtp() {
	timeb t1,t2;
    long t;
	PFC pfc(AES_SECURITY);  // initialise pairing-friendly curve

	G2 Q,V;
	G1 S,R;
	GT result;
	Big s,X;
	time_t seed;

	time(&seed);
    irand((long)seed);

	long sum=0;
    for(int i=0;i<5000;i++) {
        ftime(&t1);
        pfc.hash_and_map(R,(char *)"Test Message to sign");
        ftime(&t2);
        t=(t2.time-t1.time)*1000+(t2.millitm-t1.millitm);
        sum+=t;
    }
    cout<<"measure_mtp: "<<sum<<endl;
}

int main() {
	//execute the above functions
    measure_hash();
    measure_mtp();
    measure_bap();
    measure_gtexp();
    measure_gtmul();
    measure_bp();
    measure_bsm();
    return 0;
}