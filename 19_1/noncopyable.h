/**
 * @file   noncopyable.h
 * @author Aklzzz(csdn)
 * @date   2024/03/03 14:56:34
 * @note   
 *  继承这个基类，可以禁止其派生类的拷贝语义
*/

#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

struct Noncopyable {
  Noncopyable(const Noncopyable&) = delete;
  Noncopyable& operator=(const Noncopyable&) = delete;

protected:
  Noncopyable() = default;
  ~Noncopyable() = default;
};

#endif // NONCOPYABLE_H