      program calc_dis_xsec

C     program to calculate the DIS model/vertex cross section at one set of kinematics
c     uses the same subroutine and tables as mc_single_arm so there cannot be any mis-matches
c     between the model used in MC and the model used to get the "central" vertex cross section

      implicit none

      real*8 ebeamin,thetain,q2in,var_min,var_step
      real*8 sigdis,thrad,mp,sin2th
      real*8 Q2,xbj,W,nu
      real*8 eprime,xsecvert,xsecrad
      integer i,nvarbin,ispec,npbins,nthbins,flag
      character*80 rawname,filename,radfile,xfile
      character*132 str_line
      logical*4 iss
      logical first/.true./

C Function definitions.
      integer*4	last_char
      logical*4	rd_int,rd_real,strip

      parameter (mp=0.938272)


      write(6,*) 'Enter flag'
      write(6,*) '0=fixed theta, bin in eprime'
      write(6,*) '1=fixed theta, bin in xbj'
      write(6,*) '2=fixed Q2, bin in xbj'
      read(5,*) flag

      write(6,*) 'Enter kinematics:'
      write(6,*) 'theta (deg.) or Q2, '
      write(6,*) '<var> minimum, <var>-step, # of <var> bins',
     >     '  where <var> is Eprime (flag=0) or xbj (flag=1 or 2)'

      if(flag.eq.2) then
         read(5,*) q2in,var_min,var_step,nvarbin
      elseif(flag.eq.0 .or.flag.eq.1) then
         read(5,*) thetain,var_min,var_step,nvarbin
      else
         write(6,*) 'unkown option, stopping'
         stop
      endif
      
      write(6,*)'Enter input filename (assumed to be in infiles dir)'
      read(*,1968) rawname
 1968 format(a)
      filename = '../../infiles/'//rawname(1:last_char(rawname))//'.inp'
      write(6,*) filename,'opened'
      open(unit=1,status='old',file=filename)
      str_line = '!'
C Strip off header
      do while (str_line(1:1).eq.'!')
         read (1,1001) str_line
      enddo

      do i=1,33
         read (1,1001) str_line
      enddo

      read (1,1001,end=1000,err=1000) str_line
      write(6,*) str_line(1:last_char(str_line))
      iss = rd_real(str_line,ebeamin)
      ebeamin=ebeamin/1000.0

      do i=1,3
         read (1,1001) str_line
c         write(6,*) str_line
      enddo

!     Read in number of momentum bins in RC table
      read (1,1001,end=1000,err=1000) str_line
      write(*,*) str_line(1:last_char(str_line))
      iss = rd_int(str_line,npbins)
!     Read in number of theta bins in RC table
      read (1,1001,end=1000,err=1000) str_line
      write(*,*) str_line(1:last_char(str_line))
      iss = rd_int(str_line,nthbins)
!     Read in RC table name
      read (1,1001,end=1000,err=1000) str_line
      write(*,*) str_line(1:last_char(str_line))
      iss=strip(str_line,xfile)
      radfile='../rctables/'//xfile

 1001 format(a)
 1000 continue    
      
      radfile='../../rctables/'//xfile

      if(flag.eq.0 .or. flag.eq.1) then
         thrad = thetain*3.141592654/180.0
      endif
      ispec=1                   !doesn't matter what this is


      do i=1,nvarbin
         if(flag.eq.0) then
            eprime=var_min+(i-1)*var_step
            nu=ebeamin-eprime
            Q2=4.0*ebeamin*eprime*sin(thrad/2)**2
            xbj=Q2/2./mp/nu
            W=sqrt(-Q2+mp**2+2.0*mp*nu)
         elseif(flag.eq.1) then
            xbj=var_min+(i-1)*var_step
            eprime=ebeamin/(1.0+2.0*ebeamin/(mp*xbj)*sin(thrad/2.0)**2)
            nu=ebeamin-eprime
            Q2=4.0*ebeamin*eprime*sin(thrad/2)**2
            W=sqrt(-Q2+mp**2+2.0*mp*nu)
         elseif(flag.eq.2) then
            xbj=var_min+(i-1)*var_step
            Q2=q2in
            nu=Q2/xbj/2./mp
            eprime=ebeamin-nu
            sin2th=Q2/(4.0*ebeamin*eprime)
            thrad=2.0*asin(sqrt(sin2th))
            thetain=thrad*180.0/3.141592654
            W=sqrt(-Q2+mp**2+2.0*mp*nu)
         endif
         
         call xsec_model(ispec,thrad,eprime,radfile,npbins,nthbins,xsecvert,xsecrad)
         sigdis=xsecvert
         if(first) then
            write(6,*) 'Eprime    theta      xBj       Q2        W ',
     >        '        Cross section (ub/GeV/sr)'
            first=.false.
         endif
         write(6,1003) eprime,thetain,xbj,Q2,W,xsecvert
      enddo
 1003 format(1x,f6.4,4x,f7.4,3(4x,f6.4),4x,e11.5)
      end
