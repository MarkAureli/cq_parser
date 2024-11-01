class CqParser < Formula
    desc "Lexer and parser for the CQ programming language"
    homepage "https://github.com/MarkAureli/cq_parser"
    url "https://github.com/MarkAureli/cq_parser/archive/refs/tags/v1.0.1.tar.gz"
    sha256 "f701cd582fac2d4a87bdc7f8be0c1b8209cacc16d8ff9a68bd952d1d1f01ce3f"

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
