classdef template_class < matlabshared.ioclient.peripherals.DigitalIO ...
        & matlabshared.ioclient.peripherals.ADC ...
        & matlabshared.ioclient.peripherals.I2C ...
        & matlabshared.ioclient.peripherals.PWM ...
        & matlabshared.ioclient.peripherals.SCI
    properties 
        IOProtocol
    end
    methods
        function obj = template_class(port, varargin)    
            obj.IOProtocol = matlabshared.ioclient.IOProtocol(matlabshared.ioclient.transport.TransportLayerUtility('serial', port));
            if length(varargin) >= 2
                modifyProperties(obj.IOProtocol.TransportLayer.TransportLayerHandle, varargin);
            end
        end
        function status = connect(obj,varargin)
            status  = connect(obj.IOProtocol,varargin{:});
        end
        
        function startConfigureStreaming(obj)
            startConfigureStreaming(obj.IOProtocol);
        end
        
        function stopConfigureStreaming(obj)
            stopConfigureStreaming(obj.IOProtocol);            
        end
     
        function setRate(obj,rate)
            setRate(obj.IOProtocol,rate);
        end

        function record = startStreaming(obj,varargin)
            record = startStreaming(obj.IOProtocol,varargin{:,:});
        end
        
        function data = stopStreaming(obj)
            data = stopStreaming(obj.IOProtocol);
        end
    end
end