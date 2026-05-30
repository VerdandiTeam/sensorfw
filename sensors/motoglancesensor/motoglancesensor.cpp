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

#include "motoglancesensor.h"

#include "sensormanager.h"
#include "bin.h"
#include "bufferreader.h"

MotoGlanceSensorChannel::MotoGlanceSensorChannel(const QString &id)
    : AbstractSensorChannel(id)
    , DataEmitter<TimedUnsigned>(1)
    , m_previousValue(0, 0)
    , m_filterBin(nullptr)
    , m_marshallingBin(nullptr)
    , m_motoGlanceAdaptor(nullptr)
    , m_motoGlanceReader(nullptr)
    , m_outputBuffer(nullptr)
{
    SensorManager &sm = SensorManager::instance();

    m_motoGlanceAdaptor = sm.requestDeviceAdaptor("motoglanceadaptor");
    if (!m_motoGlanceAdaptor) {
        setValid(false);
        return;
    }

    m_motoGlanceReader = new BufferReader<TimedUnsigned>(1);

    m_outputBuffer = new RingBuffer<TimedUnsigned>(1);

    // Create buffers for filter chain
    m_filterBin = new Bin;

    m_filterBin->add(m_motoGlanceReader, "motoglance");
    m_filterBin->add(m_outputBuffer, "buffer");

    m_filterBin->join("motoglance", "source", "buffer", "sink");

    // Join datasources to the chain
    connectToSource(m_motoGlanceAdaptor, "motoglance", m_motoGlanceReader);

    m_marshallingBin = new Bin;
    m_marshallingBin->add(this, "sensorchannel");

    m_outputBuffer->join(this);

    setDescription("motoglance events");
    setRangeSource(m_motoGlanceAdaptor);
    addStandbyOverrideSource(m_motoGlanceAdaptor);
    setIntervalSource(m_motoGlanceAdaptor);

    setValid(true);
}

MotoGlanceSensorChannel::~MotoGlanceSensorChannel()
{
    if (m_motoGlanceAdaptor) {
        SensorManager &sm = SensorManager::instance();
        disconnectFromSource(m_motoGlanceAdaptor, "motoglance", m_motoGlanceReader);
        sm.releaseDeviceAdaptor("motoglanceadaptor");
        m_motoGlanceAdaptor = nullptr;
    }

    delete m_marshallingBin;
    m_marshallingBin = nullptr;

    delete m_filterBin;
    m_filterBin = nullptr;

    delete m_outputBuffer;
    m_outputBuffer = nullptr;

    delete m_motoGlanceReader;
    m_motoGlanceReader = nullptr;
}

bool MotoGlanceSensorChannel::start()
{
    qCDebug(lcSensorFw) << id() << "Starting MotoGlanceSensorChannel";

    if (AbstractSensorChannel::start()) {
        m_marshallingBin->start();
        m_filterBin->start();
        m_motoGlanceAdaptor->startSensor();
    }
    return true;
}

bool MotoGlanceSensorChannel::stop()
{
    qCDebug(lcSensorFw) << id() << "Stopping MotoGlanceSensorChannel";

    if (AbstractSensorChannel::stop()) {
        m_motoGlanceAdaptor->stopSensor();
        m_filterBin->stop();
        m_marshallingBin->stop();
    }
    return true;
}

void MotoGlanceSensorChannel::emitData(const TimedUnsigned &value)
{
    qCDebug(lcSensorFw) << id()
        << "old:" << m_previousValue.value_
        << "-->"
        << "new:" << value.value_;

    if (value.value_ != m_previousValue.value_) {
        m_previousValue.value_ = value.value_;
        //writeToClients((const void *)&value, sizeof value);
    }
    writeToClients((const void *)&value, sizeof value);
}
