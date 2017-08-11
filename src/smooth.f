        REAL FUNCTION smooth(X)
        COMMON/PAWPAR/PAR(7)
        real bg(71)
        DATA RT2PI / 2.506628274922 /
        DATA binwid / 0.005/
        Integer i
        i = (x-1.74)/0.005 + 1
        If (i.gt.71) i = 71
        If (i.lt.1) i = 1

        FIT_ALL_P2P_SWM = binwid * par(1) / (par(3)*rt2pi) * 
     &    exp(-0.5*((x-par(2))/par(3))**2) + 
     &    par(4) + PAR(5)*(X-par(2)) + par(6)*(x-par(2))**2 +
     &    par(7)*hquadf(x)


        RETURN
        END                           

