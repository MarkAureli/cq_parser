class CqParser < Formula
    desc "Lexer and parser for the CQ programming language"
    homepage "https://github.com/MarkAureli/cq_parser"
    url "https://github.com/MarkAureli/cq_parser/archive/refs/tags/v1.0.0.tar.gz"
    sha256 "29c35b1ca079ecf61ebf3d4627031353ca7f47bc0974a48f9905afe54612af0f"

    depends_on "flex"
    depends_on "bison"

    def install
        system "make"
        bin.install "cq_parser"
    end

    test do
        assert_match "1.0.0", shell_output("#{bin}/cq_parser --version")
    system "make", "test"
    end
end
