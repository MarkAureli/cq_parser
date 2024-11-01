class CqParser < Formula
    desc "Lexer and parser for the CQ programming language"
    homepage "https://github.com/MarkAureli/cq_parser"
    url "https://github.com/MarkAureli/cq_parser/archive/refs/tags/v1.0.1.tar.gz"
    sha256 "ae2ff4a6968aa157d9741ebab42408a292ff4039eb42c265a7a33c53844c146b"

    depends_on "flex"
    depends_on "bison"

    def install
        system "make"
        bin.install "cq_parser"
    end

    test do
        system "#{bin}/cq_parser", "--version"
    end
end
