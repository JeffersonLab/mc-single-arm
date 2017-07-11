      subroutine hms_hbook_init(filename,spec_ntuple)
C Initialize ntuples differently based on which spectrometer we
C are using
C
C HBOOK/NTUPLE common block and parameters.
      character*80 filename
      logical*4 spec_ntuple
      integer*4	pawc_size
      parameter	(pawc_size = 1000000)
      common		/pawc/ hbdata(pawc_size)
      integer*4	hbdata
      character*8	hut_nt_names(14)/
     >     'hsxfp', 'hsyfp', 'hsxpfp', 'hsypfp',
     >     'hsytari','hsdeltai', 'hsyptari', 'hsxptari',
     >     'hsytar', 'hsdelta', 'hsyptar', 'hsxptar', 
     >     'fry','ztari'/

      call hlimit(pawc_size)

cmkj      iquest(10) = 256000
cmkj	  iquest(10) = 510000
! see for example
!   http://wwwasd.web.cern.ch/wwwasd/cgi-bin/listpawfaqs.pl/7
! the file size is limited to ~260M no matter how I change iquest !
cmkj	  call hropen(30,'HUT',filename,'NQ',4096,i) !CERNLIB
      call hropen(30,'HUT',filename,'N',4096,i) !CERNLIB
 
      if (i.ne.0) then
         write(*,*),'HROPEN error: istat = ',i
         stop
      endif

      call hbookn(1,'HUT NTUPLE',20,'HUT',10000,hut_nt_names)
c      if (spec_ntuple) then
c         call hbookn(1412,'SPEC NTU',58,'HUT',10000,spec_nt_names)
c      endif

      return
      end
      
