module Common = {
  let base_uri = "https://api.spotify.com/v1";

  let uid_length = 22;

  type mode = [ | `album | `artist | `track];

  let string_of_mode =
    fun
    | `album => "album"
    | `artist => "artist"
    | `track => "track";

  exception Invalid_href;

  let check_href = (mode, href) =>
    try(
      Scanf.sscanf(
        href,
        "spotify:%s@:%s",
        (mode_string, uid) => {
          if (mode_string != string_of_mode(mode)) {
            raise(Invalid_href);
          };
          if (String.length(uid) != uid_length) {
            raise(Invalid_href);
          };
        },
      )
    ) {
    | Scanf.Scan_failure(_) => raise(Invalid_href)
    };
};

module Remote = {
  module C = Cohttp_lwt_unix;

  let read_uri = (uri, parse_fn) =>
    Cohttp_lwt_unix.IO.(
      C.Client.call(~chunked=false, `GET, uri)
      >>= (
        ((_, body)) =>
          Cohttp_lwt__Body.to_string(body)
          >>= (data => return(parse_fn(data)))
      )
    );
};

module Search = {
  open Lwt;

  let search = (mode, query, parse_fn) => {
    let uri =
      Uri.of_string(
        Printf.sprintf(
          "%s/search?type=%s&q=%s",
          Common.base_uri,
          Common.string_of_mode(mode),
          query,
        ),
      );

    Remote.read_uri(uri, parse_fn);
  };

  let search_albums = query =>
    search(`album, query, Album_j.search_wrapper_of_string)
    >|= (wrapper => wrapper.Album_t.albums);

  let search_artists = query =>
    search(`artist, query, Artist_j.search_wrapper_of_string)
    >|= (wrapper => wrapper.Artist_t.artists);

  let search_tracks = query =>
    search(`track, query, Track_j.search_wrapper_of_string)
    >|= (wrapper => wrapper.Track_t.tracks);
};
