module Common: {
  let base_uri: string;

  exception Invalid_href;

  type mode = [ | `album | `artist | `track];

  let string_of_mode: mode => string;

  let check_href: (mode, string) => unit;
};

module Search: {
  let search_albums:
    string => Lwt.t(Paging_t.paging(Album_t.album_simplified));
  let search_artists: string => Lwt.t(Paging_t.paging(Artist_t.artist));
  let search_tracks: string => Lwt.t(Paging_t.paging(Track_t.track));
};
