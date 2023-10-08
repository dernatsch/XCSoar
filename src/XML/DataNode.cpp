// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "DataNode.hpp"
#include "Math/Angle.hpp"
#include "util/StaticString.hxx"
#include "util/NumberParser.hpp"
#include "time/RoughTime.hpp"

ConstDataNode::~ConstDataNode() noexcept
{
}

bool
ConstDataNode::GetAttribute(const char *name, Angle &value) const noexcept
{
  double v;
  if (GetAttribute(name, v)) {
    value = Angle::Degrees(v);
    return true;
  } else
    return false;
}

bool
ConstDataNode::GetAttribute(const char *name, FloatDuration &value) const noexcept
{
  double v;
  if (GetAttribute(name, v)) {
    value = FloatDuration{v};
    return true;
  } else
    return false;
}

bool
ConstDataNode::GetAttribute(const char *name,
                            std::chrono::duration<unsigned> &value) const noexcept
{
  unsigned v;
  if (GetAttribute(name, v)) {
    value = std::chrono::duration<unsigned>{v};
    return true;
  } else
    return false;
}

bool
ConstDataNode::GetAttribute(const char *name, double &value) const noexcept
{
  const TCHAR *val = GetAttribute(name);
  if (val == nullptr)
    return false;

  value = ParseDouble(val);
  return true;
}

bool
ConstDataNode::GetAttribute(const char *name, int &value) const noexcept
{
  const TCHAR *val = GetAttribute(name);
  if (val == nullptr)
    return false;

  value = ParseInt(val);
  return true;
}

bool
ConstDataNode::GetAttribute(const char *name, unsigned &value) const noexcept
{
  const TCHAR *val = GetAttribute(name);
  if (val == nullptr)
    return false;

  value = ParseInt(val);
  return true;
}

bool
ConstDataNode::GetAttribute(const char *name, bool &value) const noexcept
{
  const TCHAR *val = GetAttribute(name);
  if (val == nullptr)
    return false;

  value = ParseInt(val) > 0;
  return true;
}

RoughTime
ConstDataNode::GetAttributeRoughTime(const char *name) const noexcept
{
  const TCHAR *p = GetAttribute(name);
  if (p == nullptr)
    return RoughTime::Invalid();

  TCHAR *endptr;
  unsigned hours = ParseUnsigned(p, &endptr, 10);
  if (endptr == p || *endptr != ':' || hours >= 24)
    return RoughTime::Invalid();

  p = endptr + 1;
  unsigned minutes = ParseUnsigned(p, &endptr, 10);
  if (endptr == p || *endptr != 0 || minutes >= 60)
    return RoughTime::Invalid();

  return RoughTime(hours, minutes);
}

RoughTimeSpan
ConstDataNode::GetAttributeRoughTimeSpan(const char *start_name,
                                         const char *end_name) const noexcept
{
  return RoughTimeSpan(GetAttributeRoughTime(start_name),
                       GetAttributeRoughTime(end_name));
}

WritableDataNode::~WritableDataNode() noexcept
{
}

void
WritableDataNode::SetAttribute(const char *name, Angle value) noexcept
{
  SetAttribute(name, value.Degrees());
}

void
WritableDataNode::SetAttribute(const char *name, double value) noexcept
{
  StaticString<48> buf;
  buf.UnsafeFormat(_T("%g"), (double)value);
  SetAttribute(name, buf);
}

void
WritableDataNode::SetAttribute(const char *name, int value) noexcept
{
  StaticString<24> buf;
  buf.UnsafeFormat(_T("%d"), value);
  SetAttribute(name, buf);
}

void
WritableDataNode::SetAttribute(const char *name, unsigned value) noexcept
{
  StaticString<24> buf;
  buf.UnsafeFormat(_T("%d"), value);
  SetAttribute(name, buf);
}

void
WritableDataNode::SetAttribute(const char *name, bool value) noexcept
{
  StaticString<4> buf;
  buf.UnsafeFormat(_T("%d"), (int)value);
  SetAttribute(name, buf);
}

void
WritableDataNode::SetAttribute(const char *name, RoughTime value) noexcept
{
  if (!value.IsValid())
    /* no-op */
    return;

  StaticString<8> buffer;
  buffer.UnsafeFormat(_T("%02u:%02u"), value.GetHour(), value.GetMinute());
  SetAttribute(name, buffer);
}
