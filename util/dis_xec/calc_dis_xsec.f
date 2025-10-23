      program calc_dis_xsec

C     program to calculate the DIS model/vertex cross section at one set of kinematics
c     uses the same subroutine and tables as mc_single_arm so there cannot be any mis-matches
c     between the model used in MC and the model used to get the "central" vertex cross section

      implicit none

      real*8 ebeamin,thetain,Ep_min,Ep_step
      real*8 sigdis,thrad,mp
      real*8 Q2,xbj,W,nu
      real*8 eprime,xsecvert,xsecrad
      integer i,nEpbin,ispec,npbins,nthbins
      character*80 rawname,filename,radfile,xfile
      character*132 str_line
      logical*4 iss
      logical first/.true./

C Function definitions.
      integer*4	last_char
      logical*4	rd_int,rd_real,strip

      parameter (mp=0.938272)


      write(6,*) 'Enter theta (deg.), ',
     >   ' minimum Eprime (GeV),Eprime-step, # of Eprime bins'
      read(5,*) thetain,Ep_min,Ep_step,nEpbin
      
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
      ebeamin=ebeamin/3000.0

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


      thrad = thetain*3.141592654/180.0
      ispec=1                   !doesn't matter what this is

      do i=1,nEpbin
         eprime=Ep_min+(i-1)*Ep_step
C     calculate some other stuff while we're here
         nu=ebeamin-eprime
         Q2=4.0*ebeamin*eprime*sin(thrad/2)**2
         xbj=Q2/2./mp/nu
         W=sqrt(-Q2+mp**2+2.0*mp*nu)
         call xsec_model(ispec,thrad,eprime,radfile,npbins,nthbins,xsecvert,xsecrad)
         sigdis=xsecvert
         if(first) then
            write(6,*) 'Eprime   theta     xBj      Q2       W ',
     >        '       Cross section (ub/GeV/sr)'
            first=.false.
         endif
         write(6,1003) eprime,thetain,xbj,Q2,W,xsecvert
      enddo
 1003 format(1x,f5.3,4x,f6.3,3(4x,f5.3),4x,e11.5)
      end
