\inc "std/stdbool.csh"

\inc "color.csh"
\inc "hittable.csh"
\inc "ray.csh"

def hit_record;

def material {
    type: int8
}

@scatter:material(r_in: ray, rec: hit_record, attenuation: color*, scattered: ray*): bool;

def lambertian {
    type: int8,
    albedo: color
}

@lambertian(albedo: color);
@scatter:lambertian(r_in: ray, rec: hit_record, attenuation: color*, scattered: ray*): bool;

def metal {
    type: int8,
    albedo: color,
    fuzz: flt64
}

@metal(albedo: color, fuzz: flt64);
@scatter:metal(r_in: ray, rec: hit_record, attenuation: color*, scattered: ray*): bool;

def dielectric {
    type: int8,
    ## Refractive index in vacuum or air, or the ratio of the material's refractive index over
    ## the refractive index of the enclosing media
    refraction_index: flt64
}

@dielectric(refraction_index: flt64);
@scatter:dielectric(r_in: ray, rec: hit_record, attenuation: color*, scattered: ray*): bool;
