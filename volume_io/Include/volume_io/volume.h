#ifndef  DEF_VOLUME
#define  DEF_VOLUME

#include  <def_mni.h>

typedef  enum  { MNC_FORMAT, FREE_FORMAT } Volume_file_types;

typedef  unsigned  char  Volume_type;

typedef  struct
{
    String        filename;
    Volume_type   ***data;
    Real          value_scale;
    Real          value_translation;
    int           sizes[N_DIMENSIONS];
    Real          thickness[N_DIMENSIONS];
    Boolean       flip_axis[N_DIMENSIONS];
    Transform     world_to_voxel_transform;
    Transform     voxel_to_world_transform;
} volume_struct;

typedef  struct
{
    Volume_file_types  file_type;

    int                sizes_in_file[N_DIMENSIONS];
    int                axis_index[N_DIMENSIONS];
    int                slice_index;

/* for mnc files only */

    int                cdfid;
    int                icv;
    Volume_type        *slice_buffer;

/* for free format files only */

    FILE               *volume_file;
    short              *short_slice_buffer;

} volume_input_struct;

#endif
