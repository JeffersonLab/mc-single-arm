      subroutine get_physics(ebeammev,pcentmev,th_rad,hsdelta,hsxptar,
     >         hsyptar,W,Q2,x_bj,
     >         xi,hstheta,eprime, ispec)

C Recalculate "physics" variables since 
C     1) mc_hms does not calculate them at all  AND
C     2) We may be applying different offsets here than we did in the replay
C     For replay variables, we pass eloss so we can retain the energy loss correction
C     For mc_hms, we use zero.
C     elosstmp is a dummy variable to avoid overlap with eloss, which is now in the 
C     kinematics.cmn common block. There are times when we may want to call this
C     routine with eloss=0

      implicit none
      

      integer i,ispec

      real*8 ebeammev,pcentmev
      real*8 ebeam,pcent,th_rad
      real*8 hsdelta,hsxptar,hsyptar
      real*8 W,Q2,x_bj,xi,hstheta,eprime
      real*8 m_electron, m_proton, m_neutron, deg_rad
      real*8 eprot
      real*8 h_oop_offset

      real*8 hoop,hp,hp_z,henergy,nu,W2
      real*8 ht(4),hk(4),hkp(4),hq(4),wvec(4)
      logical doing_shms, doing_hms

      parameter(m_electron=0.00051100)
      parameter(m_proton=0.93827231)
      parameter(m_neutron=0.93956563)
      parameter(deg_rad = 3.1415927/180.)
c      parameter(h_oop_offset=1.1E-03)
      parameter(h_oop_offset=0.0)

      ebeam=ebeammev/1000.0
      pcent=pcentmev/1000.0
      
      W = 0.0
      Q2 = 0.0
      x_bj = 0.0
      xi = 0.0
      hstheta = 0.0
      eprime = 0.0

      hoop = hsxptar + h_oop_offset
      hp = pcent*(1.0+hsdelta/100.0)
      henergy = sqrt(hp**2+m_electron**2)
      hp = sqrt(henergy**2-m_electron**2)
      hp_z = hp/sqrt(1.0+hoop**2+hsyptar**2)

      
      eprime = henergy

c      write(6,*) 'hms energy,p',henergy,hp,hp_z,hsdelta,pcent
C target
      ht(1) = m_proton
      ht(2) = 0.
      ht(3) = 0.
      ht(4) = 0.

C Beam 4vec
      hk(1) = ebeam
      hk(2) = 0.0
      hk(3) = 0.0
      hk(4) = ebeam

C Scattered electron 4vec
      hkp(1) = henergy
      hkp(2) = hp_z*hoop
C For SHMS (left side of beam)
      if(ispec.eq.2) then
         hkp(3) = hp_z*(hsyptar*cos(th_rad) + sin(th_rad))
         hkp(4) = hp_z*(-hsyptar*sin(th_rad) + cos(th_rad))
      elseif(ispec.eq.1) then
         hkp(3) = hp_z*(hsyptar*cos(th_rad) - sin(th_rad))
         hkp(4) = hp_z*(hsyptar*sin(th_rad) + cos(th_rad))
      endif

c      write(6,*) 'th_deg,th_rad', th_deg,th_rad
c      write(6,*) 'scattered electron 4 vector',ebeam_cor,hkp(1),hkp(2),hkp(3),hkp(4)

C q-vector and w-vector
      do i=1,4
         hq(i) = hk(i)-hkp(i)
         wvec(i) = ht(i) + hq(i)
c         write(6,*) 'qvec',i,hq(i)
      enddo

      W2 = wvec(1)**2-wvec(2)**2-wvec(3)**2-wvec(4)**2
      W = sqrt(W2)

      Q2 = -(hq(1)**2-hq(2)**2-hq(3)**2-hq(4)**2)

      nu = ebeam-henergy
      x_bj = Q2/(2.0*m_proton*nu)
      xi = 2.0*x_bj/(1.0+sqrt(1.+4*m_proton**2*x_bj**2/Q2))

      if(hkp(4)/hp.le.1.0) then
         hstheta = acos(hkp(4)/hp)
      else
         hstheta = -10.0
      endif


      return
      end
      








