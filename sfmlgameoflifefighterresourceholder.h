#ifndef BOOK_RESOURCEHOLDER_HPP
#define BOOK_RESOURCEHOLDER_HPP

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

///Adapted from https://github.com/SFML/SFML-Game-Development-Book
template <typename Resource, typename Identifier>
struct ResourceHolder
{
  ResourceHolder() : mResourceMap{} {}

  void Load(Identifier id, const std::string& filename);

  template <typename Parameter>
  void Load(Identifier id, const std::string& filename, const Parameter& secondParam);

  Resource& Get(Identifier id);
  const Resource& Get(Identifier id) const;


private:
  void InsertResource(Identifier id, std::unique_ptr<Resource> resource);

private:
  std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

#include "sfmlgameoflifefighterresourceholder.inl"

#endif // BOOK_RESOURCEHOLDER_HPP
