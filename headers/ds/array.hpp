#ifndef _ARRAY_HPP 
#define _ARRAY_HPP 

// TODO: use optional parameter for init() 

template <typename T> 
class Array2D 
{
    private: 
        T* ptr; 
        int nrCols, nrRows; 

    public: 
        Array2D() 
        {
            ptr = NULL; 
        }; 

        Array2D(int nrCols, int nrRows) 
        {
            ptr = NULL; 
            init(nrCols, nrRows); 
        } 

        void init(int nrCols, int nrRows) 
        {
            if(ptr) 
                free(ptr); 

            this->nrCols = nrCols; 
            this->nrRows = nrRows; 

            ptr = (T*)malloc(this->nrCols * this->nrRows * sizeof(T)); 
            if(ptr == NULL) 
            {
                exit(EXIT_FAILURE); 
            } 
        } 

        void init(int nrCols, int nrRows, T initValue) 
        {
            init(nrCols, nrRows); 
            for(int i = 0; i < nrCols*nrRows; ++i) 
                ptr[i] = initValue; 
        } 

        // getters 
        int getSize() const 
        {
            return (nrCols * nrRows); 
        } 

        int getSizeInBytes() const 
        {
            return (nrCols * nrRows * sizeof(T)); 
        } 

        int getWidth() const 
        {
            return (this->nrCols); 
        }
        
        int getHeight() const 
        {
            return (this->nrRows); 
        } 

        ~Array2D() 
        {
            if(ptr) 
            {
                free(ptr); 
                ptr = NULL; 
            } 
        } 
}; 

#endif /* _ARRAY_HPP */ 

