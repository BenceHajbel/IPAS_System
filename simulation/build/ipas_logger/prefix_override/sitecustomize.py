import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/bence-hajbel/GitHub_repose/IPAS/Simulation/install/ipas_logger'
