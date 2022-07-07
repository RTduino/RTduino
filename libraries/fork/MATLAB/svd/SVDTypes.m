classdef SVDTypes
    %SVDTYPES Enumeration class for Simulink Virtual Devices
    %   
    
    % Copyright 2015-2016 The MathWorks, Inc.

    %#codegen
    
    properties (Constant)
      MW_Input = uint8(0)
      MW_Output = uint8(1)
      MW_Off    = uint8(0)
      MW_On     = uint8(1)
      MW_High   = uint8(1)
      MW_Low    = uint8(0)
      MW_Idle   = uint8(0)
      MW_Busy   = uint8(1)
      MW_Complete = uint8(2)
      MW_Error   = uint8(3)
      MW_Master = uint8(0)
      MW_Slave  = uint8(1)
    end

    
    
    methods (Access=private)
        function obj = SVDTypes()
            coder.allowpcode('plain');
        end
    end
end

