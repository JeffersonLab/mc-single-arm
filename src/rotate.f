        program rotate

	real*8 ang,x,y,xi
	real*8 alpha,beta,alpha_p,beta_p,sin_th,cos_th,tan_th

        parameter (ang=9.2)
        parameter (x0=0)
        parameter (y0=0)

	sin_th = sin(ang)
	cos_th = cos(ang)
	tan_th = sin_th/cos_th

	alpha  = 0
	beta   = 0

	alpha_p= (alpha + tan_th)/(1. - alpha*tan_th)
	beta_p = beta/(cos_th - alpha*sin_th)

        xi = x0
	x  = xi*(cos_th + alpha_p*sin_th)
	y  = y0 + xi*beta_p*sin_th

        write(*,*) '$$$',x,y

        end
