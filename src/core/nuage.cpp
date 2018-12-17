#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>

#include "constants.h"
#include "nuage.h"

Nuage::Nuage( int n, float dmin, Distance const& distance )
    : n_( n )
    , dmin_( dmin )
    , distance_{distance}
{
}

void Nuage::generateCloud()
{
    const float sphereRadius = 1.0f;

    std::random_device rd;
    std::mt19937 gen( rd() );
    std::uniform_real_distribution<float> dis( -sphereRadius, sphereRadius );

    auto isIn = [&sphereRadius]( auto const& points ) {
        auto const x2 = points[ dirX ] * points[ dirX ];
        auto const y2 = points[ dirY ] * points[ dirY ];
        auto const z2 = points[ dirZ ] * points[ dirZ ];

        return std::sqrt( x2 + y2 + z2 ) <= sphereRadius;
    };

    while ( points_.size() < static_cast<std::size_t>( n_ ) )
    {
        glm::vec3 point{0.f, 0.f, 0.f};
        // Generate a random point until it's in the sphere.
        do
        {
            point[ dirX ] = dis( gen );
            point[ dirY ] = dis( gen );
            point[ dirZ ] = dis( gen );

        } while ( !isIn( point ) );

        // Check if this new point is not too close (< dmin_)
        // of an existing point in points_ table.
        bool should_insert
            = std::all_of( points_.begin(), points_.end(), [&point, this]( auto const& pt ) {
                  return distance_( point, pt ) >= dmin_;
              } );
        if ( should_insert )
            points_.push_back( point );
    }


    /*for(int i = 0; i < int(points_.size()); i++){
        std::cout << points_[i].x << " " << points_[i].y << " " << points_[i].z << std::endl;
    }
    std::cout << points_.size() << std::endl;*/
}

glm::vec3 Nuage::cloudBarycenter(){
    glm::vec3 barycenter_to_return{0.f, 0.f, 0.f};
    int nbpoints = points_.size();

    for(int i = 0 ; i < nbpoints; i++){
        barycenter_to_return[ dirX ] += points_[ i ][ dirX ];
        barycenter_to_return[ dirY ] += points_[ i ][ dirY ];
        barycenter_to_return[ dirZ ] += points_[ i ][ dirZ ];
    }
    barycenter_to_return[ dirX ] /= nbpoints;
    barycenter_to_return[ dirY ] /= nbpoints;
    barycenter_to_return[ dirZ ] /= nbpoints;

    std::cout << barycenter_to_return.x << " " << barycenter_to_return.y << " " << barycenter_to_return.z << std::endl;
    return barycenter_to_return;
}
