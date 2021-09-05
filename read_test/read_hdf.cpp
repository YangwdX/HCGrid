#include <iostream>
#include <string>
#include "H5Cpp.h"
#include <hdf5.h>

#ifndef _H5_NO_NAMESPACE_
using namespace H5;
#ifndef _H5_NO_STD_
    using std::cout;
    using std::endl;
#endif /* _H5_NO_STD_ */        
#endif /* _H5_NO_NAMESPACE_ */

double *h_lons;
double *h_lats;
// double *h_data;
double *h_data;
double *h_weights;
/* 
 *  Define the names of HDF5 file, groups, datasets, and attributes.
 *  Use H5::H5std_string for name strings.
 */
// const H5std_string FILE_NAME("mulDim.hdf5");
// const H5std_string GROUP_NAME("group1");
// const H5std_string DATASET_NAME("dset");
// const H5std_string ATTR_NAME("myAttr2");

void read_input_map_hdf5(const char *infile){
    hid_t file_id;// hid_t是HDF5对象id通用数据类型，每个id标志一个HDF5对象
    herr_t status; // herr_t是HDF5报错和状态的通用数据类型

    // 打开HDF5文件
    // 文件id = H5Fopen(const char *文件名
    //                  unsigned 读写flags,
    //                    - H5F_ACC_RDWR可读可写    //                    - H5F_ACC_RDONLY只读
    //                  hid_t 访问性质)
    file_id = H5Fopen(infile, H5F_ACC_RDWR, H5P_DEFAULT);

    // Number of input points.
    uint32_t numOfImputPoints = 69768; //竖扫ver19个文件每个文件采样点有 3660*19=69540 , 横扫hor 18*3876=69768
    // uint32_t numOfImputPoints = 3660;
    h_GMaps.data_shape = numOfImputPoints;
    h_GMaps.spec_dim = 618;

    //分配内存空间
    h_data = RALLOC(double, numOfImputPoints);
    h_lons = RALLOC(double, numOfImputPoints); //longitude赤经ra
    h_lats = RALLOC(double, numOfImputPoints); //latitude赤纬dec
    h_weights = RALLOC(double, numOfImputPoints); //采样权重

    // Read the data
    // 创建数据集中的数据本体    // dataset_id = H5Dopen(group位置id,
    //                 const char *name, 数据集名
    //                    数据集访问性质)
    hid_t dataset_id;
    dataset_id = H5Dopen(file_id, "/Ta", H5P_DEFAULT);
    // 将数据写入数据集
    // herr_t 读取状态= H5Dread(写入目标数据集id,
    //                              内存数据类型,
    //                       memory_dataspace_id, 定义内存dataspace和其中的选择
    //                          - H5S_ALL: 文件中dataspace用做内存dataspace，file_dataspace_id中的选择作为内存dataspace的选择
    //                         file_dataspace_id, 定义文件中dataspace的选择
    //                          - H5S_ALL: 文件中datasapce的全部，定义为数据集中dataspace定义的全部维度数�?    //  本次IO操作的转换性质,
    //                          const void * buf, 内存中接受数据的位置
    status = H5Dread(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, h_data);

    // Read the coordinates
    //赤经
    dataset_id = H5Dopen(file_id, "/ra", H5P_DEFAULT);
    status = H5Dread(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, h_lons);
    //赤纬
    dataset_id = H5Dopen(file_id, "/dec", H5P_DEFAULT);
    status = H5Dread(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, h_lats);

    // Intial the weight
    for(int i=0; i<numOfImputPoints; ++i)
        h_weights[i] = 1.;
    
    for(int i=0; i<numOfImputPoints; ++i)
        cout<<h_data[i]<<endl;
    
    // 关闭dataset相关对象
    status = H5Dclose(dataset_id);
    // 关闭文件对象
    status = H5Fclose(file_id);
}

int main (int argc, char **argv)
{
    read_input_map_hdf5("sigDim.hdf5");
    // Try block to detect exceptions raised by any of the calls inside it.
    // try
    // {
    //     /*
    // 	 * Turn off the auto-printing when failure occurs so that we can
    // 	 * handle the errors appropriately
    //      */
    // 	Exception::dontPrint();

    //     /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

    //     /* HOW TO DELETING A DATASET! */

    //     /*

    //     // Open an existing file.
    //     // H5::H5F_ACC_RDWR : read or edit an existing file.
    //     H5File file_d(FILE_NAME, H5F_ACC_RDWR);
        
    //     // Open an existing group.
    //     Group group_d = file_d.openGroup(GROUP_NAME);

    //     // Use H5::H5Ldelete to delete an existing dataset.
    //     int result = H5Ldelete(group_d.getId(), DATASET_NAME.c_str(), H5P_DEFAULT);
    //     // String.c_str() convert "string" to "const char *".

    //     cout << result << endl;     
    //     // Non-negtive: successfully delete; 
    //     // Otherwise: fail.

    //     // Save and exit the group.
    //     group_d.close();
    //     // Save and exit the file.
    //     file_d.close();
    //     // Important! The two close()s above can't be omitted! 
    //     // Otherwise, the deleting behavior won't be saved to file.

    //     */

    //     /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

    //     // Open an existing file.
    //     // H5::H5F_ACC_RDWR : read or edit an existing file.
    //     H5File file(FILE_NAME, H5F_ACC_RDWR);
        
    //     // Open an existing group of the file.
    //     // Group group = file.openGroup(GROUP_NAME);

    //     // Open an existing dataset of the group.
    //     DataSet dataset = group.openDataSet(DATASET_NAME);

    //     // Get the dataspace of the dataset.
    // 	DataSpace filespace = dataset.getSpace();

    //     // Get the rank of the dataset.
    //     int rank = filespace.getSimpleExtentNdims();

    //     // Use H5::hsize_t (similar to int) for dimensions
    //     hsize_t dims[rank];        // dataset dimensions

    //     // Get the dimensions of the dataset.
    //     rank = filespace.getSimpleExtentDims(dims);

    //     cout << DATASET_NAME << " rank = " << rank << ", dimensions "
    //              << dims[0] << " x "
    //              << dims[1] << endl;

    //     // Dataspace for data read from file.
    //     DataSpace myspace(rank, dims);

    //     double data_out[dims[0]][dims[1]];      // buffer for data read from file

    //     // Read data from file to buffer.
    //     dataset.read(data_out, PredType::NATIVE_DOUBLE, myspace, filespace);

    //     for (int i = 0; i < dims[0]; i++)
    //     {
    //         for (int j = 0; j < dims[1]; j++)
    //             cout << data_out[i][j] << " ";
    //         cout << endl;
    //     }

    //     /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
    //     // Read the attribute of the dataset.
    //     cout << endl;

    //     // Open an existing attribute of the dataset.
    //     Attribute attr = dataset.openAttribute(ATTR_NAME);

    //     // Get the dataspace of the attribute.
    //     DataSpace attr_space = attr.getSpace();

    //     // Get the rank of the attribute.
    //     int attr_rank = attr_space.getSimpleExtentNdims();

    //     // Use H5::hsize_t (similar to int) for dimensions.
    //     hsize_t attr_dims[attr_rank];       // attribute dimensions

    //     // Get the dimension of the attribute.
    //     attr_rank = attr_space.getSimpleExtentDims(attr_dims);

    //     cout << ATTR_NAME << " rank = " << attr_rank << ", dimensions " << attr_dims[0] << endl;

    //     char attr_data_out[attr_dims[0]];   // buffer for attribute data read from file

    //     // Read attribute data from file to buffer.  
    //     attr.read(PredType::NATIVE_CHAR, attr_data_out);

    //     cout << attr_data_out << endl;

    //     /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

    //     // Save and exit the group.
    //     group.close();
    //     // Save and exit the file.
    //     file.close();

    // }  // end of try block

    // // Catch failure caused by the H5File operations.
    // catch(FileIException error)
    // {
    // 	error.printErrorStack();
    // 	return -1;
    // }

    // // Catch failure caused by the DataSet operations.
    // catch(DataSetIException error)
    // {
    // 	error.printErrorStack();
    // 	return -1;
    // }

    // // Catch failure caused by the DataSpace operations.
    // catch(DataSpaceIException error)
    // {
    //     error.printErrorStack();
    //     return -1;
    // }

    return 0;  // successfully terminated
    
}