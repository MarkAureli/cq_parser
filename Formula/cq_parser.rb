class CqParser < Formula
  desc "Lexer and parser for the CQ programming language"
  homepage "https://github.com/MarkAureli/cq_parser"
  url "https://github.com/MarkAureli/cq_parser/archive/refs/tags/v1.0.0.tar.gz"
  sha256 "0019dfc4b32d63c1392aa264aed2253c1e0c2fb09216f8e2cc269bbfb8bb49b5"

  # Specify dependencies
  depends_on "flex"
  depends_on "bison"

  def install
    system "make"
    bin.install "cq_parser"
  end

  test do
    system "make", "test"
  end
end
