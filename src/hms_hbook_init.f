      subroutine hms_hbook_init(filename,spec_ntuple)
C Initialize ntuples differently based on which spectrometer we
C are using
C
C HBOOK/NTUPLE common block and parameters.
      character*80 filename
      logical*4 spec_ntuple
      integer*4 pawc_size
      parameter (pawc_size = 10000000)
      common /pawc/ hbdata(pawc_size)
      integer*4 hbdata
      common /QUEST/ iquest(100)
      integer*4 iquest
      character*8 hut_nt_names(24)/
     >  'hsxfp','hsyfp','hsxpfp','hsypfp'
     > ,'hsxtari','hsztari','hsytari','hsdeltai','hsyptari','hsxptari'
     > ,'hsztar','hsytar','hsdelta','hsyptar','hsxptar'
     > ,'fry','xsnum','ysnum','xsieve','ysieve','stop_id'
     > ,'vxi','vyi','vzi'/

      call hlimit(pawc_size)

c By Jixie: to create a large ntuple, we need to do the following 4 things:
c  1) CALL hlimit(pawc_size) with a big argument (set 'pawc_size' to a big value). 
c  2) set 'iquest(10)' to a big number, this value means the maximum records.
c     Important note: It is very important to initialise IQUEST(10) just before
c     the call to HROPEN. The QUEST common block is used a lot in the whole CERNLIB,
c     and in particular in HBOOK, so if the value of IQUEST(10) is defined too far
c     from the call to HROPEN, very likely it will not have the expected value when
c     needed. 
c  3) CALL HROPEN with a large value of LRECL (ex LRECL=4096 instead of 1024)
c     and you can get files 4 times bigger. This value means the maximum words
c     in one record.
c  4) CALL HROPEN with the option QE (E for "Extended"). This option allows to
c     have up to 2**32 records in an HBOOK file.
c  By default, no nutuple will be larger than 2G Bytes. Other place need to be
c  changed if you want that happen.
c  Check https://userweb.jlab.org/~brads/Manuals/pawfaq/ for details
!  also see for example
!   http://wwwasd.web.cern.ch/wwwasd/cgi-bin/listpawfaqs.pl/7

      iquest(10) = 512000
      call hropen(30,'HUT',filename,'NQE',4096,i) !CERNLIB
 
      if (i.ne.0) then
         write(*,*),'HROPEN error: istat = ',i
         stop
      endif

      call hbookn(1,'HUT NTUPLE',24,'HUT',10000,hut_nt_names)
c      if (spec_ntuple) then
c         call hbookn(2,'SPEC NTU',58,'HUT',10000,spec_nt_names)
c      endif

      return
      end
      
