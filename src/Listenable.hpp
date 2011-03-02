#ifndef BIM_LISTENABLE_H
#define BIM_LISTENABLE_H


namespace bim {

class EventDispatcher;

class Listenable
{
    public:
        Listenable();
        virtual ~Listenable();
        inline bool isReady() const {
            return _descriptor > 0;
        }
        inline int getDescriptor() const {
            return _descriptor;
        }
        
        /**
         * @brief Registers callbacks to the event dispatcher.
         * @param ed The event dispatcher instance
         * The base version does not register callbacks, you have to do it in
         * inherited classes with listenIn()/listenOut() and cie.
         * Will register the event dispatcher in an internal attribute and
         * returns true.
         */
        virtual bool registerEventDispatcher(EventDispatcher& ed);
        void unregisterEventDispatcher();
        virtual void onIn() {};
        virtual void onOut() {};
        virtual void onErr() {};

    protected:
        int _descriptor;
        EventDispatcher* _event_dispatcher;
};
} // /bim

#include "EventDispatcher.hpp"

#endif

