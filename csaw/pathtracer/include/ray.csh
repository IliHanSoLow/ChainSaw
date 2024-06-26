\inc "vec3.csh"

def ray {
    origin: point3,
    direction: vec3,
    time: flt64
}

@ray;
@ray(origin: point3, direction: vec3);
@ray(origin: point3, direction: vec3, time: flt64);

@at:ray(t: flt64): point3;
