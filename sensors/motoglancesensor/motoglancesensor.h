/****************************************************************************
**
** Copyright (c) 2025 Jollyboys Ltd.
**
** $QT_BEGIN_LICENSE:LGPL$
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MOTOGLANCE_SENSOR_CHANNEL_H
#define MOTOGLANCE_SENSOR_CHANNEL_H

#include <QObject>

#include "deviceadaptor.h"
#include "abstractsensor.h"
#include "motoglancesensor_a.h"
#include "dataemitter.h"
#include "datatypes/timedunsigned.h"
#include "datatypes/unsigned.h"

class Bin;
template <class TYPE> class BufferReader;
class FilterBase;

/** Sensor for accessing motoglance sensor events.
 *
 * Signals whenever motoglance sensor events are received.
 */
class MotoGlanceSensorChannel
    : public AbstractSensorChannel
    , public DataEmitter<TimedUnsigned>
{
    Q_OBJECT
    Q_PROPERTY(Unsigned motoGlance READ motoGlance NOTIFY motoGlanceChanged)

public:
    /** Factory method for MotoGlanceSensorChannel.
     *
     * @return New MotoGlanceSensorChannel as AbstractSensorChannel*
     */
    static AbstractSensorChannel *factoryMethod(const QString &id)
    {
        MotoGlanceSensorChannel *sc = new MotoGlanceSensorChannel(id);
        new MotoGlanceSensorChannelAdaptor(sc);

        return sc;
    }

    /** Property for accessing the measured value.
     *
     * Note that sensor does not have a state and thus
     * the last measured value is meaningness and is
     * provided only for the sake of symmetry.
     *
     * @return Last measured value.
     */
    Unsigned motoGlance() const { return m_previousValue; }

public Q_SLOTS:
    bool start();
    bool stop();

signals:
    /** Sent when a change in measured data is observed.
     *
     * @param value Measured value.
     */
    void motoGlanceChanged(const Unsigned &value);

protected:
    MotoGlanceSensorChannel(const QString &id);
    virtual ~MotoGlanceSensorChannel();

private:
    TimedUnsigned m_previousValue;
    Bin *m_filterBin;
    Bin *m_marshallingBin;
    DeviceAdaptor *m_motoGlanceAdaptor;
    BufferReader<TimedUnsigned> *m_motoGlanceReader;
    RingBuffer<TimedUnsigned> *m_outputBuffer;

    void emitData(const TimedUnsigned &value);
};
#endif // MOTOGLANCE_SENSOR_CHANNEL_H
