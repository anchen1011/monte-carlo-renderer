#include "Renderer.h"

#include "ArgParser.h"
#include "Camera.h"
#include "Image.h"
#include "Ray.h"
#include "VecUtils.h"
#include "Samplers.h"

#include <limits>


Renderer::Renderer(const ArgParser &args) :
    _args(args),
    _scene(args.input_file)
{
}

void
Renderer::Render()
{
    int w = _args.width;
    int h = _args.height;

    Image image(w, h);
    Image nimage(w, h);
    Image dimage(w, h);

    // loop through all the pixels in the image
    // generate all the samples

    // This look generates camera rays and calls traceRay.
    // It also write to the color, normal, and depth images.
    // You should understand what this code does.
    Camera* cam = _scene.getCamera();
    for (int y = 0; y < h; ++y) {
        float ndcy = 2 * (y / (h - 1.0f)) - 1.0f;
        for (int x = 0; x < w; ++x) {

            std::cout << x << "," << y << std::endl;

            float ndcx = 2 * (x / (w - 1.0f)) - 1.0f;
            // Use PerspectiveCamera to generate a ray.
            // You should understand what generateRay() does.
            Ray r = cam->generateRay(Vector2f(ndcx, ndcy));

            Hit h;
            Vector3f color = traceRay(r, cam->getTMin(), _args.bounces, h);

            image.setPixel(x, y, color);
            nimage.setPixel(x, y, (h.getNormal() + 1.0f) / 2.0f);
            float range = (_args.depth_max - _args.depth_min);
            if (range) {
                dimage.setPixel(x, y, Vector3f((h.t - _args.depth_min) / range));
            }
        }
    }
    // END SOLN

    // save the files 
    if (_args.output_file.size()) {
        image.savePNG(_args.output_file);
    }
    if (_args.depth_file.size()) {
        dimage.savePNG(_args.depth_file);
    }
    if (_args.normals_file.size()) {
        nimage.savePNG(_args.normals_file);
    }
}



Vector3f
Renderer::traceRay(const Ray &r,
    float tmin,
    int bounces,
    Hit &h) const
{
    // The starter code only implements basic drawing of sphere primitives.
    // You will implement phong shading, recursive ray tracing, and shadow rays.

    const float esp = 1e-4f;
    if (_scene.getGroup()->intersect(r, tmin, h)) {
        auto iAmbient = _scene.getAmbientLight();
        for (unsigned i = 0; i < 3; i++) {
            iAmbient[i] *= h.getMaterial()->getDiffuseColor()[i];
        }
        auto illumination = iAmbient;
        for (unsigned i = 0; i < _scene.getNumLights(); i++) {
            auto light = _scene.getLight(i);
            Vector3f dirToLight;
            Vector3f lightIntensity;
            float distToLight;
            auto tmpHit = Hit();
            light->getIllumination(r.pointAtParameter(h.getT()),dirToLight,lightIntensity,distToLight);
            bool ret = _scene.getGroup()->intersect(Ray(r.pointAtParameter(h.getT()),dirToLight), esp, tmpHit);
            if (_args.shadows && ret && tmpHit.getT() > esp && tmpHit.getT() * dirToLight.abs() < distToLight)
                continue;
            auto ratioLight = h.getMaterial()->shadeBrdf(r,Ray(r.pointAtParameter(h.getT()),dirToLight),h);
            //std::cout << ratioLight << " " << lightIntensity.x() << "," << lightIntensity.y() << "," << lightIntensity.z() << " " << std::max(0.0f,Vector3f::dot(-r.getDirection().normalized(),h.getNormal().normalized())) << std::endl;
            //std::cout << distToLight << " ";
            illumination += ratioLight * lightIntensity
                            * std::max(0.0f,Vector3f::dot(-r.getDirection().normalized(),h.getNormal().normalized()));
        }

        // std::cout << illumination.x() << "," << illumination.y() << "," << illumination.z() << std::endl;

        if (bounces == 0)
            return illumination;

        // auto outRs = NaiveSampler::sample(r, h);
        // std::cout << bounces << std::endl;
        auto outRs = MonteCarloSampler::sample(r, h, 30);

        for (const Ray &outR:outRs) {
            float volume = 1.0f / outRs.size();
            auto ratioOutR = h.getMaterial()->shadeBrdf(r,outR,h);
            auto tmpHit2 = Hit();
            illumination += volume * ratioOutR * traceRay(outR, esp, bounces-1, tmpHit2)
                            * std::max(0.0f,Vector3f::dot(-r.getDirection().normalized(),h.getNormal().normalized()));
        }

        return illumination;
    } else {
        return _scene.getBackgroundColor(r.getDirection());
    };


}

