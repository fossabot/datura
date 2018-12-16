#include "ellipsoid.h"

#include <algorithm>
#include <iterator>

Ellipsoid::Ellipsoid( Mode const& mode )
    : mode_{mode}
    , orientation_{Quaternion( 0, 0, 0, 0 )}
    , cloud_{createCloud( 50, 0.005 )}
    , transformation_{mode}

{
}

Nuage Ellipsoid::createCloud( int n, float dmin )
{
    Nuage cloud_to_return = Nuage( n, dmin, Distance{scale_} );
    cloud_to_return.generateCloud();
    return cloud_to_return;
}

Distance Ellipsoid::createDistance( glm::vec3 scale )
{
    return Distance( scale );
}

Nuage Ellipsoid::computeTransform()
{
    Nuage transformedNuage{cloud_};

    glm::mat4 const scaleOperation  = transformation_.scale( scale_ );
    glm::mat4 const rotateOperation = orientation_.toMat4();

    glm::mat4 const translateOperation = transformation_.translate( center_ );

    glm::mat4 const finalOperation = translateOperation * rotateOperation * scaleOperation;

    std::transform( std::cbegin( transformedNuage ), std::cend( transformedNuage ),
                    std::begin( transformedNuage ), [&finalOperation]( auto const& point ) {
                        glm::vec4 homogenePoint{point, 1.f};

                        homogenePoint = finalOperation * homogenePoint;

                        return glm::vec3{homogenePoint.x, homogenePoint.y, homogenePoint.z};
                    } );


    return transformedNuage;
}
