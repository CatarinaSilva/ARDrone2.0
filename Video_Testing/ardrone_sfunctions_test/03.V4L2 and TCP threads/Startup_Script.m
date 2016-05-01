
%------------Start Drone specific characteristics --------------%
load Config_Set.mat
%Load IMU Bus Object
LibPath = which('AR_Drone_2_Library');
MagicValue = regexp(LibPath,'AR_Drone_2_Library.slx');
load([LibPath(1:(MagicValue-1)) 'Save_IMU_struct.mat'])
%%
%------------End Drone specific characteristics --------------%




%% Sample time
%

    Sampletime=1/400;
    
    
%% observer
%


% Filter for z
    m = 0.48;
    Ac = [0 1; 0 0];
    Bc = [0; -1];
    hf = 1/400;
    sysd = c2d(ss(Ac,Bc,eye(2),[0;0]),hf);
    Ad = sysd.A; 
    Bd = sysd.B; 
    C = [1 0];
    L = dlqr(Ad',C',diag([10 10]),0.01); L = L';
    Amat = Ad-L*C;

% Filter for phi
    AcAphi = [0 1; 0 0];
    BcAphi = [0; 0];
    hfphi = 1/400;
    sysdAphi = c2d(ss(AcAphi,BcAphi,eye(2),[0;0]),hfphi);
    AdAphi = sysdAphi.A; 
    BdAphi = sysdAphi.B;            % the first state is theta second thetadot
    CAphi = eye(2);
    Rstatephi = [0.01 0.01];        % this is noise in the system. lower than this value means you are trusting the plant ie no disturbances in the plant [0.01 0.01]
    Routphi = [8 5];              % noise is output/measurement error [10 5]
    LAphi = dlqr(AdAphi',CAphi',diag(Rstatephi),diag(Routphi)); LAphi = LAphi';
    Afilterphi = AdAphi-LAphi*CAphi;
    KDphi_adjust = 1.5;
    KPphi_adjust = 4;

% Filter for theta 
    AcA = [0 1; 0 0];
    BcA = [0; 0];
    hf = 1/400;
    sysdA = c2d(ss(AcA,BcA,eye(2),[0;0]),hf);
    AdA = sysdA.A; 
    BdA = sysdA.B;            % the first state is theta second thetadot
    CA = eye(2);
    Rstate = [0.01 0.01];        % this is noise in the system. lower than this value means you are trusting the plant ie no disturbances in the plant [0.01 0.01]
    Rout = [8 5];              % noise is output/measurement error [10 5]
    LA = dlqr(AdA',CA',diag(Rstate),diag(Rout)); LA = LA';
    Afilter = AdA-LA*CA;

    
    KDtheta_adjust = 2;
    KPtheta_adjust = 4.5;
    
%% PID
%
    m = 0.48;
    hf = 0.0025;
    Ix = 0.0022375681;
    Iy = 0.002985236;
    Iz = 0.00480374;

% references
    deltazt = 2;
    deltaz = 1;
    deltapsit = 3;
    deltapsi = -1;

    deltaxt = 2;
    deltax = 1;
    deltathetat = 0.3;
    deltatheta = 1;

    deltayt = 2;
    deltay = 1;
    deltaphit = 0.3;
    deltaphi = 1;

    alpha = deltaxt/(2+sqrt(2));
    beta = sqrt(2)*alpha;
    t = alpha+beta/2;
    ft = (1/384*alpha^4 + 1/48*alpha^3*(t-alpha/2) + 1/16*alpha^2*(t-alpha/2)^2 + alpha/12*(t-alpha/2)^3-1/24*(t-alpha/2)^4);
    deltaux =  deltax/(2*ft);
    s(1) = alpha/2;
    s(2) = alpha+beta/2;
    s(3) = alpha*3/2+beta;
    s(4) = 2*alpha+beta;

    alpha = deltayt/(2+sqrt(2));
    beta = sqrt(2)*alpha;
    t = alpha+beta/2;
    ft = (1/384*alpha^4 + 1/48*alpha^3*(t-alpha/2) + 1/16*alpha^2*(t-alpha/2)^2 + alpha/12*(t-alpha/2)^3-1/24*(t-alpha/2)^4);
    deltauy =  deltay/(2*ft);
    sy(1) = alpha/2;
    sy(2) = alpha+beta/2;
    sy(3) = alpha*3/2+beta;
    sy(4) = 2*alpha+beta;


% z
    Az = [0 1; 0 0];
    Bz = [0;-1/m];
    Cz = [1 0];
    Kz = lqr(Az,Bz, diag([1 1]),0.1);
    KDzc = Kz(2); KPzc = Kz(1);
    Lz = lqr(Az',Cz', diag([10 10]),0.01); Lz = Lz';    
    eig(Az-Lz*Cz);
    eig(Az-Bz*Kz);

% psi
    Apsi = [0 1; 0 0];
    Bpsi = [0;1/Iz];
    Cpsi = [1 0];
    Kpsi = lqr(Apsi,Bpsi, diag([1 1]),1000);
    KDpsic = Kpsi(2); KPpsic = Kpsi(1);
    Lpsi = lqr(Apsi',Cpsi', diag([10 10]),0.01); Lpsi = Lpsi';    
    eig(Apsi-Lpsi*Cpsi);
    eig(Apsi-Bpsi*Kpsi);


% x
    Ax = [0 1; 0 0];
    Bx = [0;-9.81];
    Cx = [1 0];
    Kx = lqr(Ax,Bx, diag([1 1]),20);
    KDxc = Kx(2); KPxc = Kx(1); 
    Lx = lqr(Ax',Cx', diag([10 10]),0.01); Lx = Lx';    
    eig(Ax-Lx*Cx);
    eig(Ax-Bx*Kx);

% theta
    Atheta = [0 1; 0 0];
    Btheta = [0;1/Iy];
    Ctheta = [1 0];
    Ktheta = lqr(Atheta,Btheta, diag([1 0.001]),10);
    KDtheta = Ktheta(2); KPtheta = Ktheta(1);
    Ltheta = lqr(Atheta',Ctheta', diag([10 10]),0.01); Ltheta = Ltheta';    
    eig(Atheta-Ltheta*Ctheta);
    eig(Atheta-Btheta*Ktheta);

% y
    Ay = [0 1; 0 0];
    By = [0;9.81];
    Cy = [1 0];
    Ky = lqr(Ay,By, diag([1 1]),20);
    KDyc = Ky(2); KPyc = Ky(1); 
    Ly = lqr(Ay',Cy', diag([10 10]),0.01); Ly = Ly';    
    eig(Ay-Ly*Cy);
    eig(Ay-By*Ky);

% phi
    Aphi = [0 1; 0 0];
    Bphi = [0;1/Ix];
    Cphi = [1 0];
    Kphi = lqr(Aphi,Bphi, diag([1 0.001]),10);
    KDphi = Kphi(2); KPphi = Kphi(1);
    Lphi = lqr(Aphi',Cphi', diag([10 10]),0.01); Lphi = Lphi';    
    eig(Aphi-Lphi*Cphi);
    eig(Aphi-Bphi*Kphi);

