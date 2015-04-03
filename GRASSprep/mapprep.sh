r.mapcalc "rast=et_MOD16A3_ET_2000_to_2013_mean_mm_yr = MOD16A3_ET_2000_to_2013_mean * 0.1"

g.region -p rast=TRMM_2000_2013_mean_mm_hr
r.mapcalc "TRMM_2000_2013_mean_mm_yr = TRMM_2000_2013_mean_mm_hr * 24 *365.24"


# Ready to resample and fillnulls
g.region -p n=-16 s=-28 w=-68 e=-63 res=0:0:30
r.resamp.interp in=TRMM_2000_2013_mean_mm_yr out=TRMM_2000_2013_mean_mm_yr_Andes method=bicubic
r.resamp.interp in=TRMM_2000_2013_mean_mm_yr out=TRMM_2000_2013_mean_mm_yr_Andes
r.fillnulls in=et_MOD16A3_ET_2000_to_2013_mean_mm_yr out=et_MOD16A3_ET_2000_to_2013_mean_mm_yr_Andes method=bicubic

r.mapcalc "wb_modern_Andes_mm_yr = TRMM_2000_2013_mean_mm_yr_Andes - et_MOD16A3_ET_2000_to_2013_mean_mm_yr_Andes"


# Making water balance map work over srtm
r.mapcalc "wb_modern_Andes_m3_s = wb_modern_Andes_mm_yr / 1000 / 3.15569e7 * nsres() * ewres()" --o
# GRASS will make negative values positive, so fix this
# SEE BELOW FOR FLOATING POINT MATH REMINDER
#r.mapcalc "wb_gt0_modern_Andes_m3_s = wb_modern_Andes_m3_s * (wb_modern_Andes_m3_s > 0)" --o
#r.mapcalc "tmp = wb_gt0_modern_Andes_m3_s == 0" --o
#r.mapcalc "wb_gt0_modern_Andes_m3_s = wb_modern_Andes_m3_s + tmp * 10^-10" --o
r.mapcalc "wb_gt0_modern_Andes_m3_s = wb_modern_Andes_m3_s * (wb_modern_Andes_m3_s > 0) + ((wb_modern_Andes_m3_s <= 0) * 10.^-8.)" --o

# And then
r.watershed elev=srtm3 flow=wb_gt0_modern_Andes_m3_s thresh=2 drainage=draindir stream=stream accumulation=accum --o
# OR
r.watershed elev=srtm3 flow=wb_modern_Andes_m3_s thresh=2 drainage=draindir stream=streams accumulation=accum -sa --o

r.thin in=streams out=streams_thinned --o
r.to.vect in=streams_thinned out=streams type=line --o


# Add Caimancito gage point
v.edit map=CaimancitoSanFrancisco_gauge tool=create
#echo "P 1 1
# 350506.9 7374526.5
# 1 1" | v.edit -n tool=add map=CaimancitoSanFrancisco_gauge input=-
# It is around one of these two points but clearly not at either
# Not sure whether 60 arcseconds should be added or ignored
echo "P 1 1
 350506.9 7374526.5
 350506.9 7376372
 1 1" | v.edit -n tool=add map=CaimancitoSanFrancisco_gauge input=-

# water outlet Caimancito
r.water.outlet in=draindir out=CaimancitoSanFrancisco coor=348841.657073,7378080.28442 --o
#r.null map=CaimancitoSanFrancisco null=0
r.to.vect in=CaimasncitoSanFrancisco out=CaimancitoSanFrancisco type=area


v.edit tool=create map=testclip
# MAKE THE AREA FOR TESTCLIP
v.overlay ainput=streams binput=testclip op=and output=streams_clipped
v.to.points in=streams_clipped out=streams_clipped_points dmax=250 --o
# Points are in layer 2
# (Layer 1 holds whole-line properties)
# along column has distance to ultimate mounth
# This will be easy to postprocess into a distance.

# Input 

# I need to get D, B, Q, and distance from segment mouth.
# Need S and D50 to calculate b following the closure equation (33?) of Parker, 1978
v.db.addcolumn map=streams_clipped_points columns="D50 double precision, b double precision, Q double precision, S double precision, NearestClastCount varchar, DistanceFromSegmentOutlet double precision" layer=2

# DistanceFromSegmentOutlet:
# along -- CHECK! Need to post-process
# Do this later when I can use a Python script to find the along of the junctions
# see mapprep.py

# D and D50
v.distance from=streams_clipped_points from_layer=2 to=ClastCounts to_layer=1 upload=to_attr column=NearestClastCount to_column=site
v.distance from=streams_clipped_points from_layer=2 to=ClastCounts to_layer=1 upload=to_attr column=D50 to_column=D50

# Slope and width
r.slope.aspect elev=srtm3 slope=srtm3.slope_angle format=degrees
r.mapcalc "srtm3.slope = tan(srtm3.slope_angle)" # Approximately sine, which is what we want...
v.what.rast map=streams_clipped_points layer=2 raster=srtm3.slope column=S
v.db.update map=streams_clipped_points layer=2 column=b query_column="0.05535*D50/S"

# Q -- we'll just use the upstream one to feed the model
v.what.rast map=streams_clipped_points layer=2 raster=accum column=Q # m^3/s

# Need elevation too!
v.db.addcolumn map=streams_clipped_points columns="z double precision" layer=2
r.carve raster=srtm3 vector=streams output=srtm3.carved
v.what.rast map=streams_clipped_points layer=2 raster=srtm3.carved column=z


# Export handled in Python
# v.db.select map=streams_clipped_points layer=2 columns=DistanceFromSegmentOutlet,z,b,NearestClastCount where="lcat=1"

