//# [mm]: playercore.h is not the original player include,
// Here it only has minimal definitions yanked from the original
// Player library to make the laserbarcode work.

#ifndef _PLAYERCORE_H
#define _PLAYERCORE_H

// Normalize angle to domain -pi, pi
#ifndef NORMALIZE
#define NORMALIZE(z) atan2(sin(z), cos(z))
#endif

// Macro for returning array size
#ifndef ARRAYSIZE
	// Note that the cast to int is used to suppress warning about
	// signed/unsigned mismatches.
	#define ARRAYSIZE(x) (int) (sizeof(x) / sizeof(x[0]))
#endif

namespace Player{

/** @brief Data: scan (@ref PLAYER_LASER_DATA_SCAN)

The basic laser data packet.  */
typedef struct player_laser_data
{
  /** Start and end angles for the laser scan [rad].  */
  float min_angle;
  /** Start and end angles for the laser scan [rad].  */
  float max_angle;
  /** Angular resolution [rad].  */
  float resolution;
  /** Maximum range [m]. */
  float max_range;
  /** Number of range readings.  */
  uint32_t ranges_count;
  /** Range readings [m]. */
  float *ranges;
  /** Number of intensity readings */
  uint32_t intensity_count;
  /** Intensity readings. */
  uint8_t *intensity;
  /** A unique, increasing, ID for the scan */
  uint32_t id;
} player_laser_data_t;

/** @brief A pose in space */
typedef struct player_pose3d
{
  /** X [m] */
  double px;
  /** Y [m] */
  double py;
  /** Z [m] */
  double pz;
  /** roll [rad] */
  double proll;
  /** pitch [rad] */
  double ppitch;
  /** yaw [rad] */
  double pyaw;
} player_pose3d_t;



/** @brief Info on a single detected fiducial

The fiducial data packet contains a list of these.
*/
typedef struct player_fiducial_item
{
  /** The fiducial id.  Fiducials that cannot be identified get id
      -1. */
  int32_t id;
  /** Fiducial pose relative to the detector. */
  player_pose3d_t pose;
  /** Uncertainty in the measured pose . */
  player_pose3d_t upose;
} player_fiducial_item_t;


/** @brief Data: detected fiducials (@ref PLAYER_FIDUCIAL_DATA_SCAN)

The fiducial data packet (all fiducials). */
typedef struct player_fiducial_data
{
  /** The number of detected fiducials */
  uint32_t fiducials_count;
  /** List of detected fiducials */
  player_fiducial_item_t *fiducials;

} player_fiducial_data_t;



class Driver
{
  public:

    virtual ~Driver(){};

    virtual int Setup() = 0;

    virtual int Shutdown() = 0;

    virtual int ProcessMessage(player_laser_data_t * data) = 0;
};

} /* namespace Player */

#endif /* _PLAYERCORE_H */
