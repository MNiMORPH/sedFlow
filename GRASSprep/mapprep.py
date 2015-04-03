import numpy as np
from matplotlib import pyplot as plt
from grass import script as g
from operator import itemgetter

# UPDATE TO DYNAMICALLY FIND MAX LCAT
for i in range(1, 4):
  alongs = g.parse_command('v.db.select', map='streams_clipped_points', layer=2, columns='along', where='lcat='+str(i), flags='c')
  alongs = np.array(alongs.keys()).astype(float)
  localMouth_along = np.min(alongs)
  g.run_command('v.db.update', map='streams_clipped_points', layer=2, column='DistanceFromSegmentOutlet', query_column='(along - '+str(localMouth_along)+')/1000.', where='lcat='+str(i) )
  
  
# Now export
# Going to do topology by hand this time around
for i in range(1, 4):
  out = g.parse_command('v.db.select', map='streams_clipped_points', layer=2, columns='DistanceFromSegmentOutlet,z,b,NearestClastCount', where='lcat='+str(i), flags='c').keys()
  outlist = []
  outlist.append(['KilometrageUpstreamDirected', 'ElevationInM', 'ChannelWidthInM', 'StrataGrainSizeDistribution'])
  for line in out:
    outlist.append(line.split('|'))
    outlist[-1][0] = float(outlist[-1][0])
    outlist[-1][1] = float(outlist[-1][1])
    outlist[-1][2] = float(outlist[-1][2])
    outlist[-1][3]
  outlist = sorted(outlist, key=itemgetter(0), reverse=True)
  outarray = np.array(outlist)
  np.savetxt('sedFlowInput/Branch1'+('%02d' %i)+'Profile.txt', outarray, fmt='%s', delimiter='\t')


# And create input hydrographs -- just constant for now
ts = np.arange(0, 31556736*50+1, 31556736) # 10 years in seconds
#ts = np.arange(0, 1E7+1, 1E6) # About 3 months
#ts = np.arange(0, 2, 1) # About 30 years in seconds
# In future, automatically find upstream nodes
for i in [1, 2]:
  out = np.zeros((len(ts), 2))
  out[:,0] = ts
  discharges = np.array(g.parse_command('v.db.select', map='streams_clipped_points', layer=2, columns='Q', where='lcat='+str(i), flags='c').keys()).astype(float)
  # np.min is a hack (though probably always right); should really look for upstream-most node
  Qin = np.min(discharges)
  out[:,1] = Qin
  out = np.vstack(( np.array([['ElapsedSeconds',	'DischargeInM3PerS']]).astype('|S24'), out ))
  #np.savetxt('sedFlowInput/Branch1'+('%02d' %i)+'Discharge.txt', out, fmt='%s', delimiter='\t')
  np.savetxt('/home/awickert/models/sedFlow_v1.00/UpperHumahuaca_50yr_Baseline/DischargeAndOtherInputs/Branch1'+('%02d' %i)+'Discharge.txt', out, fmt='%s', delimiter='\t')
