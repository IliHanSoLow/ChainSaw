\inc "hittable.csh"
\inc "vec3.csh"

def camera {
    ## public
    aspect_ratio: flt64,        ## Ratio of image width over height
    image_width: int32,         ## Rendered image width in pixel count
    samples_per_pixel: int32,   ## Count of random samples for each pixel
    max_depth: int32,           ## Maximum number of ray bounces into scene

    vfov: flt64,                ## Vertical view angle (field of view)
    lookfrom: point3,           ## Point camera is looking from
    lookat: point3,             ## Point camera is looking at
    vup: vec3,                  ## Camera-relative "up" direction

    defocus_angle: flt64,       ## Variation angle of rays through each pixel
    focus_dist: flt64,          ## Distance from camera lookfrom point to plane of perfect focus

    ## private
    image_height: int32,        ## Rendered image height
    pixel_samples_scale: flt64, ## Color scale factor for a sum of pixel samples
    center: point3,             ## Camera center
    pixel00_loc: point3,        ## Location of pixel 0, 0
    pixel_delta_u: vec3,        ## Offset to pixel to the right
    pixel_delta_v: vec3,        ## Offset to pixel below
    u: vec3,                    ## Camera frame basis vector
    v: vec3,                    ##
    w: vec3,                    ##
    defocus_disk_u: vec3,       ## Defocus disk horizontal radius
    defocus_disk_v: vec3        ## Defocus disk vertical radius
}

@camera;
@render:camera(filename: int8*, world: hittable*): void;
