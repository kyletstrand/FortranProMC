C S.Chekanov. Pythia6 example

      PROGRAM PYT

C...Double precision and integer declarations.
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
      INTEGER PYK,PYCHGE,PYCOMP

C...EXTERNAL statement links PYDATA on most machines.
      EXTERNAL pychge,PYDATA
 

C...Commonblocks.
      COMMON/PYJETS/N,NPAD,K(4000,5),P(4000,5),V(4000,5)
      COMMON/PYDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/PYDAT3/MDCY(500,3),MDME(8000,2),BRAT(8000),KFDP(8000,5)
      COMMON/PYSUBS/MSEL,MSELPD,MSUB(500),KFIN(2,-40:40),CKIN(200)
      COMMON/PYPARS/MSTP(200),PARP(200),MSTI(200),PARI(200)
      COMMON/PYINT5/NGENPD,NGEN(0:500,3),XSEC(0:500,3)
      INTEGER PDFGUP(2),PDFSUP(2)
      Parameter (PI=3.141592654, proton=0.938272)
      integer nwdhbook, lrecl
      integer charge, MID

      integer NMXHEP
      parameter (NMXHEP=4000)
      common/HEPEVT/NEVHEP,NHEP,ISTHEP(NMXHEP),IDHEP(NMXHEP),
     &JMOHEP(2,NMXHEP),JDAHEP(2,NMXHEP),PHEP(5,NMXHEP),VHEP(4,NMXHEP)
      integer NEVHEP,NHEP,ISTHEP,IDHEP,JMOHEP,JDAHEP
      double precision PHEP,VHEP

       REAL X,Y,Z,T,mass
       integer st,id,mother1,mother2, daughter1, daughter2

      call PROMCINIT() 
C      call PROMCADDFLOAT(T,"T")

c     total events
      NVENTS=10000

C...Note that Pythia will not look at PDFGUP and PDFSUP.
      PDFGUP(1)=4
      PDFSUP(1)=46
      MSTP(52)=2 
      MSTP(51)=1000*PDFGUP(1)+PDFSUP(1)


C...Processes qqbar, gg -> ttbar.
      MSEL=6

C...Initialize for the LHC.
      CALL PYINIT('CMS','p','p',14000D0)


C...Event loop. List first few events.
      DO IEV=1,NVENTS
        CALL PYEVNT
        IF(IEV.LE.3) CALL PYLIST(1)
         if (iev.le.10  .or.
     >   iev.le.100 .and.mod(iev,10 ).eq.0 .or.
     >   iev.le.1000.and.mod(iev,100).eq.0 .or.
     >   iev.gt.1000.and.mod(iev,1000).eq.0
     >   ) then
         write(6,*)
     +   '**number of generated events=',iev
         endif

        ISUB=MSTI(1)
        Q2=PARI(21)
        W=PARI(7)
        PT=PARI(17)
        PQ=P(2,4)*P(4,4)-P(2,1)*P(4,1)-P(2,2)*P(4,2)-P(2,3)*P(4,3)
        XBJ=Q2/(2D0*PQ)
        Ybj=Q2/(s_inv*XBJ)
        Q2true=Q2
        Ytrue=Ybj
        Wtrue=W
        Xtrue=Xbj

C.... turn event into HEPMC format      
      call pyhepc(1)

      DO I=1,NHEP       
      Call PROMCADDPARTICLE(IDHEP(I),
     +     PHEP(1,I),PHEP(2,I),PHEP(3,I),PHEP(4,I),PHEP(5,I),
     +     ISTHEP(I),JMOHEP(1,I),JMOHEP(2,I),JDAHEP(1,I),
     +     JDAHEP(2,I),VHEP(1,I),VHEP(2,I),VHEP(3,I),
     +     VHEP(4,I))
      mass=PHEP(5,I)
      id=IDHEP(I)
      X=VHEP(1,I)
      Y=VHEP(2,I)
      Z=VHEP(3,I)
      T=VHEP(4,I)
      st=ISTHEP(I)
      mother1=JMOHEP(1,I)
      mother2=JMOHEP(2,I)
      daughter1=JDAHEP(1,I)
      daughter2=JDAHEP(2,I)
      charge=0
      ENDDO


*        DO I=0,N
*** select hadrons
*        IF (K(I,1).ne.1)                           GOTO 808
** reject neutrinos
*        IF (abs(K(I,2)).eq.12.or.abs(K(I,2)).eq.14.
*     +   or.abs(K(I,2)).eq.16.or.abs(K(I,2)).eq.17.
*     +   or.abs(K(I,2)).eq.18)                     GOTO 808
*        charge=pychge(K(I,2))
*        MID=K( K(I,3) ,2)
*        Call ROOTADDPARTICLE(K(I,2),P(I,1),P(I,2),P(I,3),P(I,4))
* 808    continue
*        ENDDO

c weight of this event
        Call PROMCADDEVENT(Wtrue,ISUB,Q2true)

 818    continue  
************************** End over events
       ENDDO


C...Final statistics and histograms.
      CALL PYSTAT(1)
     
      write(6,*) 'generated cross section (mb)', XSEC(0,3)
      xcross=XSEC(0,3)*1E09 ! in pb 
      write(6,*) 'generated cross section (pb)', xcross
      Rlumi=NVENTS/xcross
      write(6,*) 'generated luminocity (pb-1)', Rlumi
      write(6,*) 'Total number of generated events=', NVENTS

** get lumi
       write(6,*) 'ISR photon radiation ? '
       write(6,*) 'MSTP(11)=', MSTP(11)

C       adding statistics metadata, the '0' is for cross section error -
C       will change
       call PROMCADDSTATS(xcross,XERR,Rlumi,NVENTS)

C        call rootwrite
       call PROMCCLOSE
 
** ***************** END OF running over resolved/direct 
*******************************************************

* calculate parton/hadron level cross sections

********* cross sections     
 
        write(6,*) 'Write this file to ', anaf  


      END
