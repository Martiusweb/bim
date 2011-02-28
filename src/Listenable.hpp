#ifndef BIM_LISTENABLE_H
#define BIM_LISTENABLE_H

namespace bim {
class Listenable
{
    public:
        Listenable();
        inline bool isReady() const {
            return _descriptor > 0;
        }
        inline int getDescriptor() const {
            return _descriptor;
        }

        virtual void onIn() {};
        virtual void onOut() {};
        virtual void onErr() {};

    protected:
        int _descriptor;
};
} // /bim

#endif

