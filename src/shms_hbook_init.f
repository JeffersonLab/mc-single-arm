      subroutine shms_hbook_init(filename,spec_ntuple)
C Initialize output names differently based on which spectrometer we
C are using
C
      implicit none
      include 'hbook.inc'
      character*80 filename,specfilename
      logical spec_ntuple
      character*16	hut_nt_names(23)/
     >     'psxfp', 'psyfp', 'psxpfp', 'psypfp',
     >     'psztari','psytari', 'psdeltai', 'psyptari', 'psxptari',
     >     'psztar','psytar', 'psdelta', 'psyptar', 'psxptar', 
     >     'psxtari','fry','xsnum','ysnum','xsieve',
     >     'ysieve','stop_id',
     >     'psvxi','psvyi' /

      character*16	spec_nt_names(59)/
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
     >     's_cal_x', 's_cal_y', 's_fcal_x', 's_fcal_y', !50
     >     'sxfp', 'syfp',  !52
     >     'sdelta', 'sxptar', 'syptar', 'sxcoll', !56 
     >     'sycoll', 'sytar', 'sflag'/ !59

      integer*4 i
      integer*4	last_char

      NtupleIO=30
      NtupleSize=23

      open(NtupleIO,file=filename,form="unformatted",access="sequential")  

      write(NtupleIO) NtupleSize
      do i=1,NtupleSize
         write(NtupleIO) hut_nt_names(i)
      enddo

      if (spec_ntuple) then
         SpecNtupleIO=31
         SpecNtupleSize=59
         i=index(filename,' ')
         i=i-4
         specfilename = filename(1:i)//'spec.bin'
         open(SpecNtupleIO,file=specfilename,form="unformatted",
     >        access="sequential")  
         write(SpecNtupleIO) SpecNtupleSize
         do i=1,SpecNtupleSize
            write(SpecNtupleIO) spec_nt_names(i)
         enddo
      endif

      return
      end
      
