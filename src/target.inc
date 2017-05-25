! TARGET.INC

! Constants

	real*8		rho_Lh2, Z_Lh2, A_Lh2, X0_Lh2, X0_cm_Lh2
	parameter	(rho_Lh2 = 0.0708)
	parameter	(Z_Lh2 = 1.)
	parameter	(A_Lh2 = 1.00794)
	parameter	(X0_Lh2 = 61.28)
	parameter	(X0_cm_Lh2 = X0_Lh2/rho_Lh2)
	real*8		rho_Al, Z_Al, A_Al, X0_Al, X0_cm_Al
	parameter	(rho_Al = 2.70)
	parameter	(Z_Al = 13.)
	parameter	(A_Al = 26.98)
	parameter	(X0_Al = 24.01)
	parameter	(X0_cm_Al = X0_Al/rho_Al)
	real*8		rho_Be, Z_Be, A_Be, X0_Be, X0_cm_Be
	parameter	(rho_Be = 1.848)
	parameter	(Z_Be = 4.)
	parameter	(A_Be = 9.012182)
	parameter	(X0_Be = 65.19)
	parameter	(X0_cm_Be = X0_Be/rho_Be)
	real*8		rho_Ti, Z_Ti, A_Ti, X0_Ti, X0_cm_Ti
	parameter	(rho_Ti = 4.54)
	parameter	(Z_Ti = 22.)
	parameter	(A_Ti = 47.867)
	parameter	(X0_Ti = 16.17)
	parameter	(X0_cm_Ti = X0_Ti/rho_Ti)
        real*8          rho_mylar, Z_mylar, A_mylar, X0_mylar, X0_cm_mylar
        parameter       (rho_mylar = 1.39)
        parameter       (Z_mylar = 4.545)
        parameter       (A_mylar = 8.735)
        parameter       (X0_mylar = 39.95)
        parameter       (X0_cm_mylar = X0_mylar/rho_mylar)
        real*8          rho_kapton, Z_kapton, A_kapton, X0_kapton, X0_cm_kapton
        parameter       (rho_kapton = 1.42)
        parameter       (Z_kapton = 4.545)  ! I don't know the ave. A and Z of kapton
        parameter       (A_kapton = 8.735)  ! use mylar numbers
        parameter       (X0_kapton = 40.56)
        parameter       (X0_cm_kapton = X0_kapton/rho_kapton)
        real*8          rho_kevlar, Z_kevlar, A_kevlar, X0_kevlar, X0_cm_kevlar
        parameter       (rho_kevlar = 0.74)
        parameter       (Z_kevlar = 2.67)
        parameter       (A_kevlar = 4.67)
        parameter       (X0_kevlar = 55.2)
        parameter       (X0_cm_kevlar = X0_kevlar/rho_kevlar)
	real*8          rho_air, Z_air, A_air, X0_air, X0_cm_air
        parameter       (rho_air = 0.00121)
        parameter       (Z_air = 7.2)   !80% N2, 20% O2
        parameter       (A_air = 14.4)
        parameter       (X0_air = 36.66)
        parameter       (X0_cm_air = X0_air/rho_air)
        real*8		inch_cm, target_pi
	parameter	(inch_cm = 2.54)
! ........ stupid name to avoid conflict with definition in constants.inc
	parameter	(target_pi = 3.14159265358979)
