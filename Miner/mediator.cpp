#include "mediator.h"
#include "field.h"
#include "clock.h"
#include <QGridLayout>

Mediator::Mediator()
  : m_field(0), m_time(0) {
  m_layout = new QGridLayout;
  setLayout(m_layout);
  on_gameReset();
  on_gameReset2();
}

void Mediator::on_gameReset() {
  if (m_field) delete m_field;
  if (m_time) delete m_time;
  m_time = new Clock(this);
  m_field = new Field(this);


  connect(m_field, SIGNAL(finished()), m_time, SLOT(stop()));

  m_layout->addWidget(m_time, 0, 0, 1, 1, Qt::AlignCenter);
  m_layout->addWidget(m_field, 1, 0, 1, 1);
}

void Mediator::on_gameReset2() {
  if (m_field) delete m_field;
  if (m_time) delete m_time;
  m_time = new Clock(this);
  m_field = new Field(this);


  connect(m_field, SIGNAL(finished()), m_time, SLOT(stop()));

  m_layout->addWidget(m_time, 0, 0, 1, 1, Qt::AlignCenter);
  m_layout->addWidget(m_field, 1, 0, 1, 1);
}
