/* MINC2.H */
#ifndef _MINC2_H_		/* Avoid multiple inclusions */
#define _MINC2_H_ 1

/************************************************************************
 * CONSTANTS
 ************************************************************************/

#ifndef MI_NOERROR
/** Generic return code for successful operations. */
#define MI_NOERROR 0
#endif /* MI_NOERROR not defined */

#ifndef MI_ERROR
/** Generic return code for operations which fail for any reason. */
#define MI_ERROR (-1)
#endif /* MI_ERROR not defined */

#define MI_NATIVE    "native____"
#define MI_TALAIRACH "talairach_"
#define MI_CALLOSAL  "callosal__"

#define TRUE  1
#define FALSE 0

#define MI_DIMATTR_ALL 0
#define MI_DIMATTR_REGULARLY_SAMPLED 0x1
#define MI_DIMATTR_NOT_REGULARLY_SAMPLED 0x2

#define MI2_CHAR_LENGTH 128

#define MI2_MAX_BLOCK_EDGES 3   /* Don't do 4D or greater chunks for now */
#define MI2_CHUNK_SIZE 32	/* Length of chunk, per dimension */
#define MAX_ZLIB_LEVEL 9

#define MAX_PATH 32
#define MAX_RESOLUTION_GROUP 16
#define MAX_CD_ELEMENTS 100

#define MI2_OPEN_READ 0x0001
#define MI2_OPEN_RDWR 0x0002

/************************************************************************
 * ENUMS, STRUCTS, and TYPEDEFS
 ************************************************************************/
struct volprops_struct;
typedef struct volprops_struct volprops;
typedef volprops *mivolumeprops_t;	/* opaque pointer to volume properties */

struct dimension_struct;
typedef struct dimension_struct dimension;
typedef dimension *midimhandle_t;       /* opaque pointer to dimension properties */

struct volumehandle_struct;
typedef struct volumehandle_struct volumehandle;
typedef volumehandle *mihandle_t;      /* opaque pointer to volume handle */


typedef enum {
  MI_TYPE_BYTE = 1,		/* 8-bit signed integer */
  MI_TYPE_CHAR = 2,		/* ASCII text */
  MI_TYPE_SHORT = 3,		/* 16-bit signed integer */
  MI_TYPE_INT = 4,		/* 32-bit signed integer */
  MI_TYPE_FLOAT = 5,		/* 32-bit floating point */
  MI_TYPE_DOUBLE = 6,		/* 64-bit floating point */
  MI_TYPE_STRING = 7,		/* string */
  MI_TYPE_UBYTE = 100,		/* 8-bit unsigned integer */
  MI_TYPE_USHORT = 101,		/* 16-bit unsigned integer */
  MI_TYPE_UINT = 102,		/* 32-bit unsigned integer */
  MI_TYPE_SCOMPLEX = 1000,	/* 16-bit signed integer complex */
  MI_TYPE_ICOMPLEX = 1001,	/* 32-bit signed integer complex */
  MI_TYPE_FCOMPLEX = 1002,	/* 32-bit floating point complex */
  MI_TYPE_DCOMPLEX = 1003,	/* 64-bit floating point complex */
  MI_TYPE_UNKNOWN  = -1	  /* when the type is a non_uniform record */
} mitype_t;

typedef enum {
  MI_CLASS_REAL = 0,
  MI_CLASS_INT = 1,
  MI_CLASS_LABEL = 2,           /* enumerated data with a description for each value */
  MI_CLASS_COMPLEX = 3,
  MI_CLASS_UNIFORM_RECORD = 4,   /*aggregate datatypes consisting of multiple values */
  MI_CLASS_NON_UNIFORM_RECORD = 5
} miclass_t;

typedef enum {
    MI_DIMCLASS_ANY = 0,	/* Don't care (or unknown) */
    MI_DIMCLASS_SPATIAL = 1,	/* Space */
    MI_DIMCLASS_TIME = 2,	/* Time */
    MI_DIMCLASS_SFREQUENCY = 3,	/* Spatial frequency */
    MI_DIMCLASS_TFREQUENCY = 4,	/* Temporal frequency */
    MI_DIMCLASS_USER = 5,	/* Arbitrary user-defined axis */
    MI_DIMCLASS_RECORD = 6	/* Record as dimension */
} midimclass_t;

typedef enum {
  MI_DIMORDER_FILE      = 0,
  MI_DIMORDER_APPARENT  = 1,
} miorder_t; // dimension orders

typedef enum {
  MI_ORDER_FILE      = 0,
  MI_ORDER_APPARENT  = 1,
} mivoxel_order_t;

typedef enum {
  MI_FILE_ORDER         = 0,    /* no flip */
  MI_COUNTER_FILE_ORDER = 1,    /* flip */
  MI_POSITIVE           = 2,    /* check step if positive -> no flip,
				   negative ->flip */
  MI_NEGATIVE           = 3,    /* check step if positive -> flip,
				                 negative -> no flip */
} miflipping_t;

typedef enum {
  MI_COMPRESS_NONE = 0,
  MI_COMPRESS_ZLIB = 1,
} micompression_t;

typedef int BOOLEAN;

typedef unsigned int midimattr_t;

typedef unsigned long misize_t;

typedef struct {
  short real;
  short imag;
} miscomplex_t;

typedef struct {
  int real;
  int imag;
} miicomplex_t;

typedef struct {
  float real;
  float imag;
} mifcomplex_t;

typedef struct {
  double real;
  double imag;
} midcomplex_t;

/************************************************************************
 * FUNCTION DECLARATIONS
 ************************************************************************/

/* ATTRIBUTE/GROUP FUNCTIONS */

extern int micreate_group(mihandle_t vol, const char *path, const char *name);
extern int midelete_attr(mihandle_t vol, const char *path, const char *name);
extern int midelete_group(mihandle_t vol, const char *path, const char *name);
extern int miget_attr_length(mihandle_t vol, const char *path, 
			     const char *name, int *length);
extern int miget_attr_type(mihandle_t vol, const char *path, const char *name,
			   mitype_t *data_type);
extern int miget_attr_values(mihandle_t vol, mitype_t data_type,
			     const char *path, const char *name, 
			     int length, void *values);
extern int miset_attr_values(mihandle_t vol, mitype_t data_type,
			     const char *path, const char *name, int length,
			     const void *values);
			     
/* FREE FUNCTIONS */
extern int mifree_name(char *name_ptr);
extern int mifree_names(char **name_pptr);

/* DATA TYPE/SPACE FUNCTIONS */
extern int miget_data_class(mihandle_t vol, miclass_t *volume_class);
extern int miget_data_type(mihandle_t vol, mitype_t *volume_data_type);
extern int miget_data_type_size(mihandle_t vol, misize_t *voxel_size);
extern int miget_space_name(mihandle_t vol, char **name);
extern int miset_space_name(mihandle_t vol, const char *name);

/* DIMENSION FUNCTIONS */
extern int miget_volume_from_dimension(midimhandle_t dimension, mihandle_t *volume);
extern int micopy_dimension(midimhandle_t dim_ptr, midimhandle_t *new_dim_ptr);
extern int micreate_dimension(const char *name, midimclass_t class, midimattr_t attr, 
			      unsigned long length, midimhandle_t *new_dim_ptr);
extern int mifree_dimension_handle(midimhandle_t dim_ptr);
extern int miget_volume_dimensions(mihandle_t volume, midimclass_t class, midimattr_t attr,
				   miorder_t order, int array_length, 
				   midimhandle_t dimensions[]);
extern int miset_apparent_dimension_order(mihandle_t volume, int array_length, midimhandle_t dimensions[]);
extern int miset_apparent_dimension_order_by_name(mihandle_t volume, int array_length, char **names);
extern int miset_apparent_record_dimension_flag(mihandle_t volume, int record_flag);
extern int miget_dimension_apparent_voxel_order(midimhandle_t dimension, miflipping_t *file_order,
						miflipping_t *sign);
extern int miset_dimension_apparent_voxel_order(midimhandle_t dimension, miflipping_t flipping_order);
extern int miget_dimension_class(midimhandle_t dimension, midimclass_t *class);
extern int miset_dimension_class(midimhandle_t dimension, midimclass_t class);
extern int miget_dimension_direction_cosines(midimhandle_t dimension, 
					     double direction_cosines[3]);
extern int miset_dimension_direction_cosines(midimhandle_t dimension, 
					     const double direction_cosines[3]);
extern int miset_dimension_description(midimhandle_t dimension, const char *comments);
extern int miget_dimension_description(midimhandle_t dimension, char **comments_ptr);
extern int miget_dimension_name(midimhandle_t dimension, char **name_ptr);
extern int miset_dimension_name(midimhandle_t dimension, const char *name);
extern int miget_dimension_offsets(midimhandle_t dimension, unsigned long array_length, 
				   unsigned long start_position, double offsets[]);
extern int miset_dimension_offsets(midimhandle_t dimension, unsigned long array_length, 
				   unsigned long start_position, const double offsets[]);
extern int miget_dimension_sampling_flag(midimhandle_t dimension, BOOLEAN *sampling_flag);
extern int miset_dimension_sampling_flag(midimhandle_t dimension, BOOLEAN sampling_flag);
extern int miget_dimension_separation(midimhandle_t dimension, mivoxel_order_t voxel_order, 
				      double *separation_ptr);
extern int miset_dimension_separation(midimhandle_t dimension, double separation);
extern int miget_dimension_separations(const midimhandle_t dimensions[], 
				 mivoxel_order_t voxel_order, int array_length, 
				 double separations[]);
extern int miset_dimension_separations(const midimhandle_t dimensions[], int array_length,
				 const double separations[]);
extern int miget_dimension_size(midimhandle_t dimension, unsigned long *size_ptr);
extern int miset_dimension_size(midimhandle_t dimension, unsigned long size);
extern int miget_dimension_sizes(const midimhandle_t dimensions[], int array_length,
				 unsigned long sizes[]);
extern int miget_dimension_start(midimhandle_t dimension, mivoxel_order_t voxel_order,
				 double *start_ptr);
extern int miset_dimension_start(midimhandle_t dimension, double start_ptr);
extern int miget_dimension_starts(const midimhandle_t dimensions[], mivoxel_order_t voxel_order,
				  int array_length, double starts[]);
extern int miset_dimension_starts(const midimhandle_t dimensions[], int array_length, 
				  const double starts[]);
extern int miget_dimension_units(midimhandle_t dimension, char **units_ptr);
extern int miset_dimension_units(midimhandle_t dimension, const char *units);
extern int miget_dimension_width(midimhandle_t dimension, double *width_ptr);
extern int miset_dimension_width(midimhandle_t dimension, double width_ptr);
extern int miget_dimension_widths(midimhandle_t dimension, mivoxel_order_t voxel_order,
				  unsigned long array_length, unsigned long start_position,
				  double widths[]);
extern int miset_dimension_widths(midimhandle_t dimension, unsigned long array_length,
				  unsigned long start_position, const double widths[]);


/* VOLUME FUNCTIONS */
extern int micreate_volume(const char *filename, int number_of_dimensions,
			   midimhandle_t dimensions[],
			   mitype_t volume_type,
			   miclass_t volume_class,
			   mivolumeprops_t create_props,
			   mihandle_t *volume);
extern int miget_volume_dimension_count(mihandle_t volume, midimclass_t class,
					midimattr_t attr, int *number_of_dimensions);

extern int miopen_volume(const char *filename, int mode, mihandle_t *volume);
extern int miclose_volume(mihandle_t volume);

extern int miget_slice_scaling_flag(mihandle_t volume, 
				    BOOLEAN *slice_scaling_flag);
extern int miset_slice_scaling_flag(mihandle_t volume, 
				    BOOLEAN slice_scaling_flag);

/* VOLUME PROPERTIES FUNCTIONS */
extern int minew_volume_props(mivolumeprops_t *props);
extern int mifree_volume_props(mivolumeprops_t props);
extern int miget_volume_props(mihandle_t vol, mivolumeprops_t *props);
extern int miset_props_multi_resolution(mivolumeprops_t props, BOOLEAN enable_flag, 
				int depth);
extern int miget_props_multi_resolution(mivolumeprops_t props, BOOLEAN *enable_flag,
				int *depth);
extern int miselect_resolution(mihandle_t volume, int depth);
extern int flush_from_resolution(mihandle_t volume, int depth);
extern int miset_props_compression_type(mivolumeprops_t props, micompression_t compression_type);
extern int miget_props_compression_type(mivolumeprops_t props, micompression_t *compression_type);
extern int miset_props_zlib_compression(mivolumeprops_t props, int zlib_level);
extern int miget_props_zlib_compression(mivolumeprops_t props, int *zlib_level);
extern int miset_props_blocking(mivolumeprops_t props, int edge_count, const int *edge_lengths);
extern int miget_props_blocking(mivolumeprops_t props, int *edge_count, int *edge_lengths,
				int max_lengths);
extern int miset_props_record(mivolumeprops_t props, long record_length, char *record_name); 
extern int miset_props_template(mivolumeprops_t props, int template_flag);

/* SLICE/VOLUME SCALE FUNCTIONS */
extern int miget_slice_max(mihandle_t volume, 
			   const unsigned long start_positions[],
			   int array_length, double *slice_max);
extern int miset_slice_max(mihandle_t volume, 
			   const unsigned long start_positions[],
			   int array_length, double slice_max);
extern int miget_slice_min(mihandle_t volume, 
			   const unsigned long start_positions[],
			   int array_length, double *slice_min);
extern int miset_slice_min(mihandle_t volume, 
			   const unsigned long start_positions[],
			   int array_length, double slice_min);
extern int miget_slice_range(mihandle_t volume,
			     const unsigned long start_positions[],
			     int array_length, double *slice_max,
			     double *slice_min);
extern int miset_slice_range(mihandle_t volume, 
			     const unsigned long start_positions[],
			     int array_length, double slice_max, 
			     double slice_min);
extern int miget_volume_max(mihandle_t volume, double *slice_max);
extern int miset_volume_max(mihandle_t volume, double slice_max);
extern int miget_volume_min(mihandle_t volume, double *slice_min);
extern int miset_volume_min(mihandle_t volume, double slice_min);
extern int miget_volume_range(mihandle_t volume, double *slice_max, 
			      double *slice_min);
extern int miset_volume_range(mihandle_t volume, double slice_max, 
			      double slice_min);
/* HYPERSLAB FUNCTIONS */

extern int miget_real_value_hyperslab(mihandle_t volume,
                                      mitype_t buffer_data_type,
                                      const long start[],
                                      const long count[],
                                      void *buffer);

extern int miset_real_value_hyperslab(mihandle_t volume,
                                      mitype_t buffer_data_type,
                                      const long start[],
                                      const long count[],
                                      const void *buffer);

extern int miget_voxel_value_hyperslab(mihandle_t volume,
                                       mitype_t buffer_data_type,
                                       const long start[],
                                       const long count[],
                                       void *buffer);

extern int miset_voxel_value_hyperslab(mihandle_t volume,
                                       mitype_t buffer_data_type,
                                       const long start[],
                                       const long count[],
                                       const void *buffer);

/* CONVERT FUNCTIONS */
extern int miconvert_real_to_voxel(mihandle_t volume,
                                   const unsigned long location[],
                                   int ndims,
                                   double real_value,
                                   double *voxel_value_ptr);

extern int miconvert_voxel_to_real(mihandle_t volume,
                                   const unsigned long voxel_coords[],
                                   int ndims,
                                   double voxel_value,
                                   double *real_value_ptr);

extern int miconvert_3D_voxel_to_world(mihandle_t volume,
                                       const double voxel[],
                                       double world[]);

extern int miconvert_3D_world_to_voxel(mihandle_t volume,
                                       const double world[],
                                       double voxel[]);

extern int miget_real_value(mihandle_t volume,
                            const unsigned long coords[],
                            int ndims,
                            double *value_ptr);

extern int miset_real_value(mihandle_t volume,
                            const unsigned long coords[],
                            int ndims,
                            double value);
extern int miget_voxel_value(mihandle_t volume,
                             const unsigned long coords[],
                             int ndims,
                             double *voxel_ptr);

/* VALID functions */
extern int miget_volume_valid_max(mihandle_t volume, double *valid_max);
extern int miset_volume_valid_max(mihandle_t volume, double valid_max);
extern int miget_volume_valid_min(mihandle_t volume, double *valid_min);
extern int miset_volume_valid_min(mihandle_t volume, double valid_min);
extern int miget_volume_valid_range(mihandle_t volume,
                                    double *valid_max, double *valid_min);
extern int miset_volume_valid_range(mihandle_t volume, 
                                    double valid_max, double valid_min);



#endif /* _MINC2_H_ */