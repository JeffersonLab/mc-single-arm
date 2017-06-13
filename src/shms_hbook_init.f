      subroutine shms_hbook_init(filename,spec_ntuple)
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
      character*8	hut_nt_names(20)/
     >     'psxfp', 'psyfp', 'psxpfp', 'psypfp',
     >     'psztari','psytari', 'psdeltai', 'psyptari', 'psxptari',
     >     'psztar','psytar', 'psdelta', 'psyptar', 'psxptar', 
     >     'psxtari','fry','xsnum','ysnum','xsieve'
     >     ,'ysieve'/

      character*8	spec_nt_names(58)/
     >     's_hb1_x', 's_hb1_y','s_hb2_x', 's_hb2_y','s_hb3_x', 
     >     's_hb3_y','s_hb4_x', 's_hb4_y', 's_q1_x', 's_q1_y', ! 10
     >     's_q2_x', 's_q2_y', 's_q3_x', 's_q3_y', !14
     >     's_d1_x', 's_d1_y', 's_d1f_x', 's_d1f_y', !18
     >     's_dme_x', 's_dme_y', 's_dm1_x', 's_dm1_y', !22
     >     'S_dm2_x', 's_dm2_y', 'S_dm3_x','s_dm3_y', !26
     >     'S_dm4_x', 's_dm4_y', 'S_dm5_x','s_dm5_y', !30
     >     'S_dm6_x', 's_dm6_y', 'S_dm7_x','s_dm7_y', !34
     >     'S_dmex_x', 's_dmex_y', 's_dex_x', 's_dex_y', !38
     >     's_dc1_x', 's_dc1_y', 's_dc2_x', 's_dc2_y', !42
     >     's_s1_x', 's_s1_y', 's_s2_x', 's_s2_y', !46
     >     's_cal_x', 's_cal_y', 's_fcal_x', 's_fcal_y',
     >     'sxfp', 'syfp', 
     >     'sdelta', 'sxptar', 'syptar', 'sxcoll', 
     >     'sycoll', 'sflag'/


      call hlimit(pawc_size)

cmkj      iquest(10) = 256000
cmkj	  iquest(10) = 510000
! see for example
!   http://wwwasd.web.cern.ch/wwwasd/cgi-bin/listpawfaqs.pl/7
! the file size is limited to ~260M no matter how I change iquest !
cmkj	  call hropen(30,'HUT',filename,'NQ',4096,i) !CERNLIB
      call hropen(30,'HUT',filename,'N',1024,i) !CERNLIB
 
      if (i.ne.0) then
         write(*,*),'HROPEN error: istat = ',i
         stop
      endif

      call hbookn(1411,'HUT NTUPLE',20,'HUT',10000,hut_nt_names)
      if (spec_ntuple) then
         call hbookn(1412,'SPEC NTU',58,'HUT',10000,spec_nt_names)
      endif

      return
      end
      
